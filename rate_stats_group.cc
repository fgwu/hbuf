#include <iostream>
#include <algorithm>
#include "global.h"
#include "disk.h"
#include "rate_stats_group.h"

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
	int group_id = upper_bound(SEG_POINT.begin(), SEG_POINT.end(),
				   p.second) - SEG_POINT.begin();
	zone_group[group_id].push_back(p.first);
	group_ratio[group_id] += sqrt_s;
    }

    // sort(m.begin(), m.end(), [](const auto& p1, const auto& p2){
    // 	    return p1.second > p2.second;});

    // for (int i = 0; i < 10 && i < m.size(); i++) {
    // 	printf("[%4u]%3luMB %4.1lfZ  ", m[i].first,
    // 	       zone_inject_size[m[i].first] / 1048576,
    // 	       100 * m[i].second / total_sqrt);
    // 	//	cout << m[i].first << " ";w
    // }
    for (int i = 0; i < SEG_POINT.size() + 1; i++) {
	sort(zone_group[i].begin(), zone_group[i].end());
	vector<zone_t> diff(zone_group[i].size() +
			    zone_group_prev[i].size(), -1);
	auto it = set_symmetric_difference(zone_group[i].begin(),
					   zone_group[i].end(),
					   zone_group_prev[i].begin(),
					   zone_group_prev[i].end(),
					   diff.begin());
	int size = it - diff.begin();
	
    	printf("[%1u]%3luZ %4.1lfZ %3.0lf%% ",
	       i,
	       zone_group[i].size(),
    	       100 * group_ratio[i]/ total_sqrt,
	       100 - (double)size * 100 /(double)diff.size());
    }
    
    cout << "\n";

    zone_inject_size.clear();
    accu_size = 0;

    swap(prev_update_set, update_set);
    update_set.clear();

    swap(zone_group, zone_group_prev);
    zone_group.clear();
    group_ratio.clear();
    return true;
}
