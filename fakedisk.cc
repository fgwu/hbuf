#include <iostream>
#include <cassert>
#include "global.h"
#include "disk.h"
#include "fakedisk.h"
#include "policy.h"

using namespace std;

FakeDisk::FakeDisk() {
    total_zone_num = TOTAL_ZONE_NUM;
    wp.resize(total_zone_num, 0);
    for (long i = 0; i < TOTAL_ZONE_NUM; i++) {
	wp[i] = i * ZONE_SIZE;
	//	cout << "wp[" << i << "]=" << wp[i] << "\n";
    }
    //    cout << "FakeDisk::FakeDisk" << " " << total_zone_num << "\n";
}


FakeDisk::~FakeDisk() {
}


// return value: the resultant logic offset. -1 indicates fail.
// for both seq and non-seq write, return the old wp.
// TODO maintain the content map.
loff_t FakeDisk::write(ioreq req){
    zone_t zone = req.off / ZONE_SIZE;
    // currently all write will be redirected to media cache for verification
    //    if (req.off != wp[zone]) {
    if (true) { 
	mc.write(req);
	return wp[zone];
    }

    // zone overflow.
    if (wp[zone] + req.len >= (zone + 1) * ZONE_SIZE ) assert(0);

    loff_t loff = wp[zone];
    wp[zone] += req.len;
    return loff;
}

size_t FakeDisk::read(ioreq req){
    // fake drive, always success with full requested length.
    return req.len;
}


bool FakeDisk::resetWritePointer(zone_t zone){
    wp[zone] = zone * ZONE_SIZE;
    return true;
}

loff_t FakeDisk::getWritePointer(zone_t zone) {
    return wp[zone];
}


zone_t FakeDisk::getZoneTotalNum() {
    return TOTAL_ZONE_NUM;
}

void FakeDisk::cleanup() {
     mc.cleanup();
}
