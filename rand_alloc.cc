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
#include <cstdlib>

using namespace std;

long rand_alloc(vector<long>& hist, int hbuf_num) {
    if (!hbuf_num) return -1;
    int n = hist.size();
    if (!hist.size()) return 0;
    vector<long> cost(hbuf_num, 0);
    vector<long> cnt(hbuf_num, 0);
    long total_clean_cost = 0;

    for (long s: hist) {
	int hbuf_idx = rand() % hbuf_num;
	// printf("%ld --> %d", s, hbuf_idx);
	long presum = cnt[hbuf_idx] ? cost[hbuf_idx] / cnt[hbuf_idx]: 0;
	cost[hbuf_idx] = (presum + s) * (++cnt[hbuf_idx]);
	// printf("    cost[%d]=%ld \n", hbuf_idx, cost[hbuf_idx]);
    }

    for (long c: cost)
	total_clean_cost += c;


    return total_clean_cost;
}

