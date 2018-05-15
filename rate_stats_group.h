// -*-c++-*-
#ifndef _RATE_STATS_GROUP_H_
#define _RATE_STATS_GROUP_H_

#include <unordered_map>
#include <unordered_set>
#include "disk.h"

const vector<long long> SEG_POINT = {1048576, 10485760, 104857600};

class RateStats{
private:
    size_t window_size;
    size_t accu_size;
    size_t total_accu_size;

    unordered_set<long long> update_set;
    unordered_set<long long> prev_update_set;

    unordered_map<zone_t, size_t> zone_inject_size;

    unordered_map<int, vector<zone_t>> zone_group;
    unordered_map<int, vector<zone_t>> zone_group_prev;
    unordered_map<int, double> group_ratio;

    bool InUpdateSet(ioreq);
public:
    RateStats(size_t);
    bool recordReq(ioreq);
};


#endif  // _RATE_STATS_GROUP_H_
