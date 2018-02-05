#include <climits>
#include <algorithm>
#include <cmath>
#include "global.h"
#include "policy_hwindow.h"
#include "hbuf.h"

Policy_Hwindow::Policy_Hwindow() {
    accu_size = 0;
}

Policy_Hwindow::~Policy_Hwindow() {}

void Policy_Hwindow::
optAllocItr(vector<vector<double>>&cost,
	    vector<vector<double>>&dp,
	    vector<vector<pair<long, long>>>&cut) {
    int n = dp[0].size();
    for (long i = 0; i < HBUF_NUM; i++)
	for (long j = 0; j < n; j++) {
	    if (!i) {
		dp[i][j] = cost[i][j];
		cut[i][j] = make_pair(0, 1);
		continue;
	    }

	    if (!j) {
		dp[i][j] = cost[0][j] / (i + 1);
		cut[i][j] = make_pair(j, i + 1);
		continue;
	    }

	    dp[i][j] = cost[0][j] / (i + 1);
	    cut[i][j] = make_pair(0, i + 1);

	    for (long k = 1; k <= j; k++)
		for (long l = 1; l <= i; l++) {
		    double cand = cost[k][j] / l + dp[i - l][k - 1];
		    if (cand >= dp[i][j]) continue;
		    dp[i][j] = cand;
		    cut[i][j] = make_pair(k, l);
		}
	}
}

double Policy_Hwindow::
optAllocRec(vector<vector<double>>&cost,
	    vector<vector<double>>&dp,
	    vector<vector<pair<long, long>>>&cut,
	    long i, long j) {
    if (dp[i][j] >= 0) return dp[i][j];
    if (!i) {
	dp[i][j] = cost[i][j];
	cut[i][j] = make_pair(0, 1);
	return dp[i][j];
    }

    if (!j) {
	dp[i][j] = cost[0][j] / (i + 1);
	cut[i][j] = make_pair(j, i + 1);
	return dp[i][j];
    }

    dp[i][j] = cost[0][j] / (i + 1);
    cut[i][j] = make_pair(0, i + 1);

    // if (i > j) {
    // 	for (long k = 1; k <= j; k++){
    // 	    long a = 1, b = i;
    // 	    long a1 = a + (b - a) * 0.382;
    // 	    long b1 = a + (b - a) * 0.618;
    // 	    while (b - a >= 4) {
    // 		double canda = cost[k][j] / a1 +
    // 		    optAllocRec(cost, dp, cut, i - a1, k - 1);

    // 		double candb = cost[k][j] / b1 +
    // 		    optAllocRec(cost, dp, cut, i - b1, k - 1);

    // 		if (canda > candb) {
    // 		    a = a1;
    // 		    a1 = b1;
    // 		    b1 = a + (b - a) * 0.618;
    // 		} else {
    // 		    b = b1;
    // 		    b1 = a1;
    // 		    a1 = a + (b - a) * 0.382;
    // 		}
    // 	    }
	
    // 	    for (long l = a; l <= b; l++) {
    // 		double cand = cost[k][j] / l +
    // 		    optAllocRec(cost, dp, cut, i - l, k - 1);
    // 		// printf("i=%ld j=%ld k=%ld l=%ld cand = %lf \n",
    // 		//        i, j, k, l, cand);
    // 		if (cand >= dp[i][j]) continue;
    // 		dp[i][j] = cand;
    // 		cut[i][j] = make_pair(k, l);
    // 	    }
    // 	}
    // } else {
    // 	for (long l = 1; l <= i; l++) {
    // 	    long a = 1, b = j;
    // 	    long a1 = a + (b - a) * 0.382;
    // 	    long b1 = a + (b - a) * 0.618;
    // 	    while (b - a >= 4) {
    // 		double canda = cost[a1][j] / l +
    // 		    optAllocRec(cost, dp, cut, i - l, a1 - 1);

    // 		double candb = cost[b1][j] / l +
    // 		    optAllocRec(cost, dp, cut, i - l, b1 - 1);
		
    // 		if (canda > candb) {
    // 		    a = a1;
    // 		    a1 = b1;
    // 		    b1 = a + (b - a) * 0.618;
    // 		} else {
    // 		    b = b1;
    // 		    b1 = a1;
    // 		    a1 = a + (b - a) * 0.382;
    // 		}
    // 	    }
	
    // 	    for (long k = a; k <= b; k++){
    // 		double cand = cost[k][j] / l +
    // 		    optAllocRec(cost, dp, cut, i - l, k - 1);
    // 		// printf("i=%ld j=%ld k=%ld l=%ld cand = %lf \n",
    // 		//        i, j, k, l, cand);
    // 		if (cand >= dp[i][j]) continue;
    // 		dp[i][j] = cand;
    // 		cut[i][j] = make_pair(k, l);
    // 	    }
    // 	}
    // }
    for (long k = 1; k <= j; k++){
    	double cand_min = cost[0][j];
    	for (long l = 1; l <= i; l++) {
    	    //	for (long l = i; l >= 1; l--) {
    	    if ((j - k + 1) * (i - l + 1) > k * l) continue;
    	    if (dp[i - l][k - 1] < 0)
    		optAllocRec(cost, dp, cut, i - l, k - 1);
    	    double cand = cost[k][j] / l + dp[i - l][k - 1];
    	    if (cand > cand_min) break;
    	    cand_min = cand;
    	    // printf("i=%ld j=%ld k=%ld l=%ld cand = %lf \n",
    	    // 	   i, j, k, l, cand);

    	    if (cand >= dp[i][j]) continue;
    	    dp[i][j] = cand;
    	    cut[i][j] = make_pair(k, l);
    	}
    }
    return dp[i][j];
}

long Policy_Hwindow::optAlloc() {
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

    // dp[i][j] stores the min cost of HBUF_NUM = i + 1, hist[0~j]
    // negative means uncalculated.
    vector<vector<double>> dp(HBUF_NUM, vector<double>(n, -1));
    vector<vector<pair<long, long>>> cut(HBUF_NUM, vector<pair<long, long>>(n));
    vector<vector<double>> mincost(HBUF_NUM, vector<double>(n, 0));

    //    optAllocItr(cost, dp, cut);
    optAllocRec(cost, dp, cut, HBUF_NUM - 1, n - 1);

    long i = HBUF_NUM - 1, j = n - 1;
    while(j >= 0) {
	long k = cut[i][j].first, l = cut[i][j].second;
    	for (long r = k; r <= j; r++) {
	    printf("zone %d mapped to hbuf %ld - %ld\n",
		   hist[r].second, i + 1 - l, i);
    	    zone_hbuf_map[hist[r].second] = make_pair(i + 1 - l, i); // hzone[start, end]
	}

	i -= l;
	j = k - 1;
    }

    printf("estimated cost: %lf\n", dp[(long)HBUF_NUM - 1][n - 1]);
    return dp[(long)HBUF_NUM - 1][n - 1];
}


double Policy_Hwindow::calcDiff(unordered_map<zone_t, double> stat_est,
		     unordered_map<zone_t, double> stat_seg){
    double tmp = 0;
    for (auto p: stat_est) {
	if (stat_seg.count(p.first))
	    tmp += (stat_seg[p.first] - p.second) *
		(stat_seg[p.first] - p.second);
	else
	    tmp += p.second * p.second;
    }

    for (auto p: stat_seg) {
	if (!stat_est.count(p.first))
	    tmp += p.second * p.second;
    }

    return sqrt(tmp) / SEG_SIZE;
}

void Policy_Hwindow::recordReq(ioreq req){
    zone_t zone = req.off / ZONE_SIZE;
    stat_seg[zone] += req.len;
    accu_size += req.len;

    if (accu_size <= SEG_SIZE) return;
    //    printf("accu_size %ld reaches segment size %ld\n", accu_size, SEG_SIZE);

    double vec_diff = calcDiff(stat_est, stat_seg);
    bool phase_change = vec_diff > BETA;

    //    printf("vec_diff = %lf ", vec_diff);
    // calculate new stat_est = stat_est * ALPHA + stat_seg * (1 - ALPHA)
    unordered_map<zone_t, double> stat_est_new;
    for (auto p: stat_est)
	stat_est_new[p.first] = p.second * ALPHA;

    for (auto p: stat_seg) {
	if (stat_est_new.count(p.first))
	    stat_est_new[p.first] += p.second * (1 - ALPHA);
	else
	    stat_est_new[p.first] = p.second * (1 - ALPHA);
    }

    swap(stat_est_new, stat_est);

    stat_seg.clear();
    accu_size = 0;
    
    if (!phase_change) return;
    //    printf("exceeds BETA=%lf re-calculate rules\n", BETA);
    
    hist.clear();
    zone_hbuf_map.clear();

    for (auto p: stat_est)
	hist.emplace_back(p.second, p.first); // reversed pair (size, zone)
    //    printf("hist size=%lu\n", hist.size());

    sort(hist.begin(), hist.end());

    // calculate zone_hbuf_map again.
    optAlloc();
    //    printf("zone_map size=%lu\n", zone_hbuf_map.size());
}

zone_t Policy_Hwindow::PickHBuf(HBuf* hbuf, ioreq req) {
    UNUSED(hbuf);
    zone_t zone = req.off / ZONE_SIZE;
    recordReq(req);

    if (!zone_hbuf_map.count(zone)) return zone % HBUF_NUM;

    // default round robin
    // if (!zone_hbuf_map.count(zone)) {
    // 	if (rr >= HBUF_NUM) rr = 0;
    // 	return rr++;
    // }

    zone_t start = zone_hbuf_map[zone].first;
    zone_t end = zone_hbuf_map[zone].second;
    zone_t z;
    for (z = start; z < end; z++) {
	if (hbuf->disk->getWritePointer(z) + req.len < (z + 1) * ZONE_SIZE)
	    break;
    }

    return z;
}
