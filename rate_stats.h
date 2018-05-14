// -*-c++-*-
#ifndef _RATE_STATS_H_
#define _RATE_STATS_H_

#include <unordered_map>
#include <unordered_set>
#include "disk.h"

class RateStats{
private:
    size_t window_size;
    size_t accu_size;
    size_t total_accu_size;

    unordered_set<long long> update_set;
    unordered_set<long long> prev_update_set;

    unordered_map<zone_t, size_t> zone_inject_size;

    bool InUpdateSet(ioreq);
public:
    RateStats(size_t);
    bool recordReq(ioreq);
};


#endif  // _RATE_STATS_H_
