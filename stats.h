#ifndef _STATS_H_
#define _STATS_H_

#include <unordered_map>
#include <cstdlib>
#include "global.h"

/*
 * This class have all the statistics: cost, etc.
 */
using namespace std;

class Stats {
 private:
    unsigned long num_zone_cleaned; // read-modify-write due to hbuf cleaning
    size_t bytes_written; // the orignial size written
    static Stats* stats;

    // correctness verification
    unordered_map<zone_t, size_t> original_inflow;
    unordered_map<zone_t, size_t> hbuf_inflow;
    unordered_map<zone_t, size_t> media_inflow;
 public:
    static Stats* getStats();
    Stats();
    ~Stats();
    void countZoneClean(int num_zones);
    void countBytesWritten(size_t len);
    void countOriginal(zone_t z, size_t s);
    void countHBuf(zone_t z, size_t s);
    void countMedia(zone_t z, size_t s);
    bool checkSize();
    void report();
};

#endif

