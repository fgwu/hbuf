#ifndef _STATS_H_
#define _STATS_H_

#include "global.h"

/*
 * This class have all the statistics: cost, etc.
 */
class Stats {
 private:
    unsigned long num_zone_cleaned; // read-modify-write due to hbuf cleaning
    size_t bytes_written; // the orignial size written
    static Stats* stats;
 public:
    static Stats* getStats();
    Stats();
    ~Stats();
    void countZoneClean(int num_zones);
    void countBytesWritten(size_t len);
    void report();
};

#endif

