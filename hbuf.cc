#include <iostream>
#include <cassert>
#include "global.h"
#include "disk.h"
#include "fakedisk.h"
#include "hbuf.h"
#include "policy.h"
#include "stats.h"

using namespace std;

HBuf::HBuf () {
    disk = new FakeDisk();
    hbuf_num = HBUF_NUM;
    uzone_num = disk->getZoneTotalNum() - hbuf_num;
    hbuf_map.resize(hbuf_num);
    //    cout << "HBuf::HBuf" << " " <<  hbuf_num  << "\n";
}

HBuf::~HBuf () {
    delete disk;
    //    cout << "HBuf::~HBuf" << hbuf_num << "\n";
}

void HBuf::cleanHBuf(zone_t buf) {
    assert(buf < hbuf_num);
    // printf("\ncleaning hbuf %u wp(%lx) size(%lu):", buf,
    // 	   disk->getWritePointer(buf), hbuf_map[buf].size());
    // for (zone_t z: hbuf_map[buf])
    // 	printf("%u ", z);
    // printf("\n");
    Stats::getStats()->countZoneClean(hbuf_map[buf].size());
    hbuf_map[buf].clear();
    disk->resetWritePointer(buf);
    // printf("cleaned  hbuf %u wp(%lx) size(%lu)\n", buf,
    // 	   disk->getWritePointer(buf), hbuf_map[buf].size());
}

// return value: the resultant logic offset. -1 indicates fail.
loff_t HBuf::writeToHBuf(ioreq req, zone_t buf){
    assert(buf < hbuf_num);
    if (disk->getWritePointer(buf) + req.len >= (buf + 1) * ZONE_SIZE)
	cleanHBuf(buf);
    
    hbuf_map[buf].insert(req.off / ZONE_SIZE);
    req.off = disk->getWritePointer(buf);
    disk->write(req);
    return req.off;
}

// return value: the resultant logic offset. 
void HBuf::write(ioreq req){
    Policy p = Policy::GetPolicy();
    writeToHBuf(req, p.PickHBuf(req));
    Stats::getStats()->countBytesWritten(req.len);
    // TODO insert poff mapping
    //    loff_t poff = writeToHBuf(req, p.PickHBuf(req));

}

void HBuf::read(ioreq req) {
    // TODO now ignore read request.
    //    printf("read req off=%lx, len=%lx\n", req.off, req.len);
    (void)req; // supressing unused req warning.
    return;
}
