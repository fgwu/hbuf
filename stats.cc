#include <iostream>
#include "stats.h"

using namespace std;

Stats::Stats(){ num_zone_cleaned = 0; cout << "Stats:Stats\n";}


Stats::~Stats(){ cout << "Stats:~Stats\n";}



void Stats::countZoneClean(int num_zones){
    num_zone_cleaned += num_zones;
    printf("stats: cleaned %d zones (accu %lu)\n", num_zones, num_zone_cleaned);
}
