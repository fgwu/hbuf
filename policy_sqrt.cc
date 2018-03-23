#include <climits>
#include <algorithm>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iostream>
#include "global.h"
#include "policy_sqrt.h"
#include "hbuf.h"

using namespace std;

Policy_Sqrt::Policy_Sqrt(): is_init(true), accu_size(0), cand(0) {
    max_win_size = 1024L * 1024 * 256 * 100 * 1024;
    win_size = 1024L * 1024 * 4096 * 1024;
}

Policy_Sqrt::~Policy_Sqrt() {}

void Policy_Sqrt::UpdateMapping () {
    zone_hbuf_map.clear();
    hbuf_cursor.clear();
    
    vector<pair<zone_t, double>> sqrt_vec;
    double total_sqrt = 0;
    for (auto p: zone_inject_size) {
	//	if (!p.first) assert(0);
	cout << p.first << "," << p.second << "\n";
	double sq_root = sqrt(p.second);
	sqrt_vec.push_back(make_pair(p.first, sq_root));
	total_sqrt += sq_root;
    }

    sort(sqrt_vec.begin(), sqrt_vec.end(),
	 [](const pair<zone_t, double> &a, const pair<zone_t, double> &b){
	     return a.second > b.second;
	});

    double accu_sqrt = 0;
    for (unsigned int i = 0; i < sqrt_vec.size(); i++){
	cout << "i=" << i << " " << sqrt_vec[i].first << ", " << sqrt_vec[i].second << "\n";
	zone_t z = sqrt_vec[i].first;
	//	if (!z) assert(0);
	cout << "zone: " << z;
	zone_hbuf_map[z].first = HBUF_NUM * accu_sqrt / total_sqrt;
	cout << " first=" << zone_hbuf_map[z].first
	     << " (" << (HBUF_NUM * accu_sqrt / total_sqrt) <<") ";
	hbuf_cursor[z] = zone_hbuf_map[z].first;
	accu_sqrt += sqrt_vec[i].second;
	zone_hbuf_map[z].second = HBUF_NUM * accu_sqrt / total_sqrt - 1;
	zone_hbuf_map[z].second = max(zone_hbuf_map[z].second,
				      zone_hbuf_map[z].first);
	cout << " second=" << zone_hbuf_map[z].second
	     << " (" << (HBUF_NUM * accu_sqrt / total_sqrt) << ")\n";
    }

    for (auto p: zone_hbuf_map)
	cout << "zone:" << p.first << ": <" << p.second.first << ","
	     << p.second.second << ">\n";

    zone_inject_size.clear();
    accu_size = 0;
}

void Policy_Sqrt::recordReq(ioreq req){
    zone_t zone = req.off / ZONE_SIZE;
    zone_inject_size[zone] += req.len;
    accu_size += req.len;

    if (is_init) return;
    if (accu_size <= win_size) return;
        
    printf("accu_size %ld reaches window size %ld\n", accu_size, win_size);
    win_size = min(win_size * 2 , max_win_size);

    UpdateMapping();
}

zone_t Policy_Sqrt::PickHBuf(HBuf* hbuf, ioreq req) {
    UNUSED(hbuf);
    zone_t zone = req.off / ZONE_SIZE;

    recordReq(req);
    
    if (is_init) {
	if (cand < HBUF_NUM &&
	hbuf->disk->getWritePointer(cand) + req.len <
	(cand + 1) * ZONE_SIZE) {
	    return cand;
	}
	is_init = false;
	printf("init pass complete, Updating mapping\n");
	UpdateMapping();
    }

    // does not appear in previous window
    // fall back to set associative
    if (!zone_hbuf_map.count(zone)) return zone % HBUF_NUM;
    
    zone_t start = zone_hbuf_map[zone].first;
    zone_t end = zone_hbuf_map[zone].second;
    zone_t h = hbuf_cursor[zone];

    // if current cursor points to an hbuf that is full, move to the next
    if (hbuf->disk->getWritePointer(h) + req.len >= (h + 1) * ZONE_SIZE) {
	if (++h > end)
	    h = start;
	hbuf_cursor[zone] = h;
	cout << "mapping zone " << zone << " ==> " << h << "\n";
    }

    //    cout << "mapping zone " << zone << " ==> " << h << "\n";
    return h;
}
