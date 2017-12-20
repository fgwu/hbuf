#include <iostream>
#include "stats.h"

using namespace std;

Stats* Stats::stats = nullptr;

Stats::Stats(){
    num_zone_cleaned = 0;
    bytes_written = 0;
}

Stats::~Stats(){
}

Stats* Stats::getStats() {
    if (stats == nullptr)
	stats = new Stats();
    return stats;
}

void Stats::countZoneClean(int num_zones){
    num_zone_cleaned += num_zones;
    //    printf("stats: cleaned %d zones (accu %lu)\n", num_zones, num_zone_cleaned);
}

void Stats::countBytesWritten(size_t len){
    bytes_written += len;
}

void Stats::report() {
    printf("total_bytes(GB) %12.3f\n",
	   bytes_written * 1.0/(1024 * 1024 * 1024));
    printf("clean_bytes(GB) %12.3f\n",
	   num_zone_cleaned * (ZONE_SIZE * 1.0/(1024 * 1024 * 1024)));
    printf("clean_overhead: %12.3f\n", num_zone_cleaned * ZONE_SIZE * 1.0 / bytes_written);
       

}
