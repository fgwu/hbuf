#include <iostream>
#include <cassert>
#include "global.h"
#include "disk.h"
#include "fakedisk.h"
#include "hbuf.h"
#include "policy.h"
#include "stats.h"

using namespace std;

HBuf::HBuf (Policy* p) {
    disk = new FakeDisk();
    policy = p;
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
    printf("\ncleaning hbuf %u wp(%lx) containing %lu zones:", buf,
    	   disk->getWritePointer(buf), hbuf_map[buf].size());
    for (auto p: hbuf_map[buf]) {
	zone_t z = p.first;
	size_t data_size = p.second;
    	printf("[%u=>%.2fMB](", z, data_size * 1.0 / (1024 * 1024));
	for (zone_t nbr_buf: zone_hbuf_map[z]) {
	    if (nbr_buf == buf) continue; // skip the current hbuf.
	    printf("hbuf%u %u->%.2fMB, ", nbr_buf, z,
		   hbuf_map[nbr_buf][z] * 1.0 / (1024 * 1024));
	    hbuf_map[nbr_buf].erase(z);
	}
	printf(")");
    }
    printf("\n");
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
    zone_t zone = req.off / ZONE_SIZE;
    hbuf_map[buf][zone] += req.len;
    zone_hbuf_map[zone].insert(buf);

    req.off = disk->getWritePointer(buf);
    disk->write(req);
    return req.off;
}

// return value: the resultant logic offset. 
void HBuf::write(ioreq req){
    writeToHBuf(req, policy->PickHBuf(this, req));
    Stats::getStats()->countBytesWritten(req.len);
    // TODO insert poff mapping
    //    loff_t poff = writeToHBuf(req, p.PickHBuf(req));

}

void HBuf::read(ioreq req) {
    // TODO now ignore read request.
    //    printf("read req off=%lx, len=%lx\n", req.off, req.len);
    UNUSED(req); // supressing unused req warning.
    return;
}
