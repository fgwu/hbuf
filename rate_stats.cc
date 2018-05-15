#include <iostream>
#include <algorithm>
#include "global.h"
#include "disk.h"
#include "rate_stats.h"

RateStats::RateStats(size_t ws) {
    //    cout << "RateStats: ws = " << ws;
    window_size = ws;
    accu_size = 0;
    total_accu_size = 0;
}


bool RateStats::InUpdateSet(ioreq req) {
    if (!req.is_write) return false;

    long long start_lba = req.off / BLK_SIZE;
    long long lba_len = req.len / BLK_SIZE;
    bool found = false;
    
    for (long long i = 0; i < lba_len; i++) {
	if (prev_update_set.count(start_lba + i))
	    found = true;
	update_set.insert(start_lba + i);	    
    }

    return found;
}

bool RateStats::recordReq(ioreq req) {

    // if (!InUpdateSet(req)) return false;
    
    zone_t zone = req.off / ZONE_SIZE;
    zone_inject_size[zone] += req.len;
    accu_size += req.len;
    total_accu_size += req.len;

    if (accu_size <= window_size) return false;
    printf("%6luMB ", total_accu_size/1048576);
    // cout << "accu_size " << accu_size << " reaches window size "
    // 	 << window_size << "\n";

    vector<pair<zone_t, double>> m;
    double total_sqrt = 0;
    
    for (auto p: zone_inject_size) {
	double sqrt_s = sqrt((double)p.second);
	m.push_back(make_pair(p.first, sqrt_s));
	total_sqrt += sqrt_s;
    }

    sort(m.begin(), m.end(), [](const auto& p1, const auto& p2){
	    return p1.second > p2.second;});

    for (int i = 0; i < 10 && i < m.size(); i++) {
	printf("[%4u]%3luMB %4.1lfZ  ", m[i].first,
	       zone_inject_size[m[i].first] / 1048576,
	       100 * m[i].second / total_sqrt);
	//	cout << m[i].first << " ";
    }

    cout << "\n";

    zone_inject_size.clear();
    accu_size = 0;

    prev_update_set = update_set;
    update_set.clear();
    return true;
}
