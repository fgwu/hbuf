#include <iostream>
#include "global.h"
#include "disk.h"
#include "policy.h"

using namespace std;

Disk::Disk () {
    hbuf_num = HBUF_NUM;
    total_zone_num = TOTAL_ZONE_NUM;
    wp.resize(TOTAL_ZONE_NUM, 0);
    for (long i = 0; i < TOTAL_ZONE_NUM; i++) {
	wp[i] = i * ZONE_SIZE;
	//	cout << "wp[" << i << "]=" << wp[i] << "\n";
    }
    hwp.resize(HBUF_NUM, 0);
    for (long i = 0; i < HBUF_NUM; i++){
	hwp[i] = i * ZONE_SIZE;
	//	cout << "hwp[" << i << "]=" << hwp[i] << "\n";		
    }
    cout << "Disk::Disk" << " " <<  hbuf_num  << " " << total_zone_num << "\n";
}


Disk::~Disk () {
    cout << "Disk::~Disk" << hbuf_num << "\n";
}


// return value: the resultant logic offset. 
loff_t Disk::write(ioreq req){
    Policy p = Policy::GetPolicy();
    return writeToHBuf(req, p.PickHBuf(req));
}


// return value: the resultant logic offset. -1 indicates fail.
loff_t Disk::writeToZone(ioreq req, zone_t zone){
    if (wp[zone] + req.len >= (zone + 1) * ZONE_SIZE ) return -1;
    //TODO indexing table update
    loff_t loff = wp[zone];
    cout << "zone:" << zone << " wp=" << wp[zone] << " len=" << req.len << "\n";
    if (wp[zone] < 0) return -1;
    wp[zone] += req.len;
    return loff;
}


// return value: the resultant logic offset. -1 indicates fail.
loff_t Disk::writeToHBuf(ioreq req, zone_t buf){
    if (hwp[buf] + req.len >= (buf + 1) * ZONE_SIZE ) return -1;
    //TODO indexing table update
    loff_t loff = hwp[buf];
    cout << "buf:" << buf << " hwp=" << hwp[buf] << " len=" << req.len << "\n";
    if (hwp[buf] < 0) return -1;
    hwp[buf] += req.len;
    return loff;
}
