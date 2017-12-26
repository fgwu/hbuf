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

void Stats::countOriginal(zone_t z, size_t s) {
    original_inflow[z] += s;
}

void Stats::countHBuf(zone_t z, size_t s) {
    hbuf_inflow[z] += s;
}
void Stats::countMedia(zone_t z, size_t s){
    media_inflow[z] += s;
}

// check the total clean data equals the origitnal data inflow
bool Stats::checkSize() {
    bool flag = true;
    for (auto p: original_inflow) {
	zone_t z = p.first;
	size_t s = p.second;
	if (s != hbuf_inflow[z] + media_inflow[z]) {
	    printf("zone %d mismatch: %lu != %lu + %lu\n",
		   z, s, hbuf_inflow[z], media_inflow[z]);
	    printf("checkInflowSize missed.\n");
	    flag = false;;
	}
    }
    return flag;
}


void Stats::report() {
    if (!checkSize())
	printf("warning: cleaning traffice not equal to original traffice size\n");
    else
        printf("checkInflowSize passed.\n");

    printf("total_bytes(GB) %12.3f\n",
	   bytes_written * 1.0/(1024 * 1024 * 1024));
    printf("clean_bytes(GB) %12.3f\n",
	   num_zone_cleaned * (ZONE_SIZE * 1.0/(1024 * 1024 * 1024)));
    printf("clean_overhead: %12.3f\n", num_zone_cleaned * ZONE_SIZE * 1.0 / bytes_written);

}
