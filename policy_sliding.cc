#include <climits>
#include <algorithm>
#include "global.h"
#include "policy_sliding.h"
#include "hbuf.h"

Policy_Sliding::Policy_Sliding(size_t ws) {
    window_size = ws;
    accu_size = 0;
}

Policy_Sliding::~Policy_Sliding() {}

long Policy_Sliding::optAlloc() {
    if (!hist.size()) return 0;
    long n = hist.size(); 
    // cost[i,j] stores the bhuf cost if it contains
    // all hists between i to j.
    vector<vector<long>> cost(n, vector<long>(n, 0)); 
    
    for (long i = 0; i < n; i++)
	for (long j = i; j < n; j++) {
	    long last_len = j - i;
	    long presum = last_len ? cost[i][j - 1] / last_len : 0;
	    cost[i][j] = (presum + hist[j].first) * (last_len + 1);
	}

    // dp[i][j] stores the min cost of HBUF_NUM < i (ie i - 1), hist[0~j]
    vector<vector<long>> dp(HBUF_NUM, vector<long>(n, 0));
    vector<vector<long>> cut(HBUF_NUM, vector<long>(n, 0));
    vector<vector<long>> mincost(HBUF_NUM, vector<long>(n, 0));

    for (long i = 0; i < HBUF_NUM; i++)
	for (long j = 0; j < n; j++) {
	    if (!i) {
		dp[i][j] = cost[i][j];
		continue;
	    }

	    if (j < i) continue;
	    dp[i][j] = LONG_MAX;
	    long cand = -1;
	    for (long k = i; k <= j; k++) {
		if (dp[i][j] > dp[i - 1][k - 1] + cost[k][j])
		    cand = k;
		dp[i][j] = min(dp[i][j], dp[i - 1][k - 1] + cost[k][j]);
	    }
	    cut[i][j] = cand;
	}

    long i = min((long)HBUF_NUM, n) - 1, j = n - 1;
    for (; i >= 0; j = cut[i][j] - 1, i--) {
	// printf("i=%ld [%ld %ld] cost=%ld\n", i, cut[i][j], j,
	//        cost[cut[i][j]][j]);
	for (zone_t k = cut[i][j]; k <= j; k++) {
	    zone_hbuf_map[hist[k].second] = i; // zone k is assined to hbuf i
	    // printf("zone %u mapped to hbuf %ld\n", hist[k].second, i);
	}
    }
    
    return dp[min(n, (long)HBUF_NUM) - 1][n - 1];
}

void Policy_Sliding::recordReq(ioreq req){
    zone_t zone = req.off / ZONE_SIZE;
    zone_inject_size[zone] += req.len;
    accu_size += req.len;

    if (accu_size <= window_size) return;
    printf("accu_size %ld reaches window size %ld\n", accu_size, window_size);
    
    // now accumulated data exceeds the window size, time to start new window
    hist.clear();
    zone_hbuf_map.clear();

    for (auto p: zone_inject_size)
	hist.emplace_back(p.second, p.first); // reversed pair (size, zone)

    sort(hist.begin(), hist.end());

    // calculate zone_hbuf_map again.
    optAlloc();
    printf("zone_map size=%lu\n", zone_hbuf_map.size());
    
    zone_inject_size.clear();
    accu_size = 0;
}

zone_t Policy_Sliding::PickHBuf(HBuf* hbuf, ioreq req) {
    UNUSED(hbuf);
    zone_t zone = req.off / ZONE_SIZE;
    // does not appear in previous window
    // fall back to set associative
    // if(zone_hbuf_map.count(zone))
    // 	printf("zone %5u -> hbuf %5u\n", zone, zone_hbuf_map[zone]);
    zone_t h = zone_hbuf_map.count(zone) ?
	zone_hbuf_map[zone] : zone % HBUF_NUM;

    // keep a record for next window
    recordReq(req);
    
    return h;
}
