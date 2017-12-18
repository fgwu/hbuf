#ifndef _STATS_H_
#define _STATS_H_

#include "global.h"

/*
 * This class have all the statistics: cost, etc.
 */
class Stats {
 private:
    unsigned long num_zone_cleaned; // read-modify-write due to hbuf cleaning
 public:
    Stats();
    ~Stats();
    void countZoneClean(int num_zones);
};


#endif

