#include <climits>
#include <algorithm>
#include <algorithm>
#include <cmath>
#include <cassert>
#include "global.h"
#include "policy_multilog.h"
#include "hbuf.h"

Policy_Multilog::Policy_Multilog(size_t ws) {
    max_win_size = ws;
    win_size = 1;
    accu_size = 0;
}

Policy_Multilog::~Policy_Multilog() {}

long Policy_Multilog::optAlloc() {
    if (!hist.size()) return 0;
    long n = hist.size(); 
    // cost[i,j] stores the bhuf cost if it contains
    // all hists between i to j.
    vector<vector<double>> cost(n, vector<double>(n, 0)); 
    
    for (long i = 0; i < n; i++)
	for (long j = i; j < n; j++) {
	    long last_len = j - i;
	    double presum = last_len ? cost[i][j - 1] / last_len : 0;
	    cost[i][j] = (presum + hist[j].first) * (last_len + 1);
	}

    // dp[i][j] stores the min cost of HBUF_NUM < i (ie i - 1), hist[0~j]
    vector<vector<double>> dp(HBUF_NUM, vector<double>(n, 0));
    vector<vector<long>> cut(HBUF_NUM, vector<long>(n, 0));
    vector<vector<double>> mincost(HBUF_NUM, vector<double>(n, 0));

    for (long i = 0; i < HBUF_NUM; i++)
	for (long j = 0; j < n; j++) {
	    if (!i) {
		dp[i][j] = cost[i][j];
		continue;
	    }

	    if (!j) {
		dp[i][j] = cost[0][j] / (i + 1);
		continue;
	    }

	    dp[i][j] = cost[0][n - 1]; // max cost
	    // if hist[j] is grouping with k, k + 1, ..., j - 1
	    // to fit in one h-zone
	    for (long k = 1; k <= j; k++) {
		double cand = dp[i - 1][k - 1] + cost[k][j];
		if (cand >= dp[i][j]) continue;
		dp[i][j] = cand;
		cut[i][j] = k;
	    }

	    // if hist[j] is taking up l h-zones 
	    // l == 1 has already been taken care of in the loop above
	    for (long l = 2; l <= i; l++) {
		double cand = dp[i - l][j - 1] + cost[j][j] / l;
		if (cand >= dp[i][j]) continue;
		dp[i][j] = cand;
		cut[i][j] = -l;
	    }
	}

    long i = (long)HBUF_NUM - 1, j = n - 1;
    
    //    for (; i >= 0; j = cut[i][j] - 1, i--) {
    while(i >= 0) {
	if (cut[i][j] >= 0) {
	    long k = cut[i][j];
	    for (long r = k; r <= j; r++) {
		printf("zone %d mapped to hbuf %ld\n",
		       hist[r].second, i);
		zone_hbuf_map[hist[r].second] = make_pair(i, i);
	    }
	    j = k - 1;
	    i--;
	} else {
	    long l = -cut[i][j];
	    printf("zone %d mapped to hbuf %ld - %ld\n",
		   hist[j].second, i + 1 - l, i);
	    zone_hbuf_map[hist[j].second] = make_pair(i + 1 - l, i);
	    j = j - 1;
	    i -= l;
	}
    }
    
    return dp[(long)HBUF_NUM - 1][n - 1];
}


double Policy_Multilog::calcDiff(vector<pair<double, zone_t>> hist_old,
				 vector<pair<double, zone_t>> hist_new){
    // double tmp = 0;
    // double base = 0;
    // for (auto p: hist_old) {
    // 	if (hist_new.count(p.second))
    // 	    tmp += (hist_new[p.second] - p.second) *
    // 		(hist_new[p.second] - p.second);
    // 	else
    // 	    tmp += p.second * p.second;
    // }

    // for (auto p: hist_new) {
    // 	if (!hist_old.count(p.second))
    // 	    tmp += p.second * p.second;
    // 	base += p.second * p.second;
    // }

    // return sqrt(tmp / base);
    assert(0);
    return 0;
}

void Policy_Multilog::recordReq(ioreq req){
    zone_t zone = req.off / ZONE_SIZE;
    zone_inject_size[zone] += req.len;
    accu_size += req.len;

    if (accu_size <= max_win_size) return;
    printf("accu_size %ld reaches window size %ld\n", accu_size, win_size);
    
    // now accumulated data exceeds the window size, time to start new window
    vector<pair<double, zone_t>> hist_new;

    for (auto p: zone_inject_size)
	hist_new.emplace_back(p.second, p.first); // reversed pair (size, zone)

    double diff = calcDiff(hist, hist_new) < 0.2;
    if (diff < 0.2)
	win_size = min(max_win_size, win_size * 2);
    else if (diff > 0.7)
	win_size = max((size_t)1, win_size / 2);

    printf("diff = %lf, win_size = %ld\n", diff, win_size);
    

    swap(hist, hist_new);
    
    sort(hist.begin(), hist.end());

    // calculate zone_hbuf_map again.
    zone_hbuf_map.clear();
    optAlloc();
    printf("zone_map size=%lu\n", zone_hbuf_map.size());
    
    zone_inject_size.clear();
    accu_size = 0;
}

zone_t Policy_Multilog::PickHBuf(HBuf* hbuf, ioreq req) {
    UNUSED(hbuf);
    zone_t zone = req.off / ZONE_SIZE;

    recordReq(req);

    // does not appear in previous window
    // fall back to set associative
    if (!zone_hbuf_map.count(zone)) return zone % HBUF_NUM;
    
    zone_t start = zone_hbuf_map[zone].first;
    zone_t end = zone_hbuf_map[zone].second;
    zone_t z;
    for (z = start; z < end; z++) {
	if (hbuf->disk->getWritePointer(z) + req.len < (z + 1) * ZONE_SIZE)
	    break;
    }
    
    return z;
}
