//#include "opt.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <functional>
#include <climits>

using namespace std;

long opt(vector<long>& hist, int hbuf_num) {
    if (!hbuf_num) return -1;
    if (!hist.size()) return 0;
    
    long n = hist.size(); 
    // cost[i,j] stores the bhuf cost if it contains
    // all hists between i to j.
    vector<vector<long>> cost(n, vector<long>(n, 0)); 

    for (long i = 0; i < n; i++)
	for (long j = i; j < n; j++) {
	    long last_len = j - i;
	    long presum = last_len ? cost[i][j - 1] / last_len : 0;
	    cost[i][j] = (presum + hist[j]) * (last_len + 1);
	}

    // for (auto & v: cost) {
    // 	for (auto c: v) printf("%5ld", c);
    // 	cout << "\n";
    // }
    // cout << "\n";
    
    // hbuf_num is enough to hold single LBA per hbuf
    if (hbuf_num >= hist.size()) return cost[0][n - 1];
    
    // dp[i][j] stores the min cost of hbuf_num < i (ie i - 1), hist[0~j]
    vector<vector<long>> dp(hbuf_num, vector<long>(n, 0));
    vector<vector<long>> cut(hbuf_num, vector<long>(n, 0));
    vector<vector<long>> mincost(hbuf_num, vector<long>(n, 0));

    for (long i = 0; i < hbuf_num; i++)
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
    cout << "\n";    

    // for (auto & v: dp) {
    // 	for (auto c: v) printf("%5ld", c);
    // 	cout << "\n";
    // }
    // cout << "\n";

    for (long i = hbuf_num - 1, j = n - 1; i >= 0; j = cut[i][j] - 1, i--) 
	printf("i=%ld [%ld %ld] cost=%ld\n", i, cut[i][j], j,
	       cost[cut[i][j]][j]);

    cout << "\n";

    return dp[hbuf_num - 1][n - 1];
}

// int main(){
//     vector<long> hist = {1, 2, 3, 4, 5, 6, 7, 8, 9};
//     cout << opt(hist, 3) << "\n";
//     return 0;
// }
