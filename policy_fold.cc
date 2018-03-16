#include <climits>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "global.h"
#include "policy_fold.h"
#include "hbuf.h"

Policy_Fold::Policy_Fold() {
    //    folded = false;
    folded = true;
    full_cnt = 0;
}

Policy_Fold::~Policy_Fold() {}

long Policy_Fold::optAlloc() {
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

    long b; // boundary [0, b), [b, n)
    for (b = 1; b < n; b++)
	if (cost[0][b - 1] * b > cost[b][n - 1] * (n - b)) break;

    assert(b > 0 && b < n - 1);
    // for (b = 1; b < n; b++)
    // 	cout << "(" << hist[b].first << "," << hist[b].second << ") ";
    // cout << "b = " << b << ", n = " << n << "\n";
    
    // dp[i][j] stores the min cost of HBUF_NUM < i (ie i - 1), hist[0~j]
    vector<vector<double>> dp(HBUF_NUM / 2, vector<double>(b, 0));
    vector<vector<long>> cut(HBUF_NUM / 2, vector<long>(b, 0));
    vector<vector<double>> mincost(HBUF_NUM / 2, vector<double>(b, 0));

    for (long i = 0; i < HBUF_NUM / 2; i++) {
	for (long j = 0; j < b; j++) {
	    if (!i) {
		dp[i][j] = cost[i][j];
		continue;
	    }

	    if (!j) {
		dp[i][j] = cost[0][j] / (i + 1);
		continue;
	    }

	    dp[i][j] = cost[0][n - 1]; // max cost
	    // if fitting hist k, k + 1, ..., j into one hbuf
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
    }

    // for (long j = 0; j < b; j++){ 
    // 	for (long i = 0; i < HBUF_NUM / 2; i++)
    // 	    printf("%4ld", cut[i][j]);
    // 	cout << "\n";
    // }

    // cout << "----------------------\n";

    long i = (long)HBUF_NUM / 2 - 1, j = b - 1;

    while(i >= 0) {
	if (cut[i][j] >= 0) {
	    long k = cut[i][j];
	    for (long r = k; r <= j; r++) {
		// printf("zone %d (%ld) mapped to hbuf %ld\n",
		//        hist[r].second, hist[r].first, i);
		//		cout << hist[r].second << " " << hist[r].first << " "
		//		     << i << "\n";
		zone_hbuf_map[hist[r].second] = make_pair(i, i);
	    }
	    j = k - 1;
	    i--;
	} else {
	    long l = -cut[i][j];
	    // printf("zone %d (%ld) mapped to hbuf %ld - %ld\n",
	    // 	   hist[j].second, hist[j].first, i + 1 - l, i);
	    //	    cout << hist[j].second << " " << hist[j].first << " "
	    //		 << i + 1 - l << " " << i << "\n";
	    zone_hbuf_map[hist[j].second] = make_pair(i + 1 - l, i);
	    j = j - 1;
	    i -= l;
	}
    }
    
    return dp[(long)HBUF_NUM / 2 - 1][b - 1];
}

void Policy_Fold::recordReq(ioreq req){
    zone_t zone = req.off / ZONE_SIZE;
    zone_inject_size[zone] += req.len;
}

void Policy_Fold::fold() {
    assert(0);
}

void Policy_Fold::calc() {
    hist.clear();
    zone_hbuf_map.clear();

    for (auto p: zone_inject_size)
	hist.emplace_back(p.second, p.first); // reversed pair (size, zone)

    sort(hist.rbegin(), hist.rend());

    // calculate zone_hbuf_map again.
    optAlloc();
    printf("zone_map size=%lu\n", zone_hbuf_map.size());
    
    //    zone_inject_size.clear();

    if (!folded) {
	fold();
	folded = true;
    }
}

zone_t Policy_Fold::PickHBuf(HBuf* hbuf, ioreq req) {
    zone_t zone = req.off / ZONE_SIZE;

    // unfolded
    // [            Module           ]
    // 0                            H_BUF
    //
    // folded
    // [   OPT group   | Module      ]
    // 0           H_BUF/2          H_BUF

    zone_t h;

    // keep a record for next window
    recordReq(req);
    
    if (!folded)
	h = zone % HBUF_NUM;
    else if (!zone_hbuf_map.count(zone))
	h = zone % (HBUF_NUM / 2) + HBUF_NUM / 2;
    else if (zone_hbuf_map[zone].first >= HBUF_NUM / 2)
	h = zone % (HBUF_NUM / 2) + HBUF_NUM / 2;
    else {
	zone_t start = zone_hbuf_map[zone].first;
	zone_t end = zone_hbuf_map[zone].second;
	for (h = start; h < end; h++) {
	    if (hbuf->disk->getWritePointer(h) + req.len < (h + 1) * ZONE_SIZE)
		break;
	}
    }

    bool full = hbuf->disk->getWritePointer(h) + req.len >=
	(h + 1) * ZONE_SIZE;
    
    if (!full) return h;

    if (folded && (h < HBUF_NUM / 2 - 1)) return h;
    // if the last OPT hbuf or any module group is full, recalc.

    if (full_cnt++ < 100) return h;
    full_cnt = 0;

    calc();

    // the map has already changed. So map it again.
    
    // zone has already be counted by recordReq()
    if (zone_hbuf_map[zone].first >= HBUF_NUM / 2)
	h = zone % (HBUF_NUM / 2) + HBUF_NUM / 2;
    else {
	zone_t start = zone_hbuf_map[zone].first;
	zone_t end = zone_hbuf_map[zone].second;
	for (h = start; h < end; h++) {
	    if (hbuf->disk->getWritePointer(h) + req.len < (h + 1) * ZONE_SIZE)
		break;
	}
    }
    
    return h;
}
