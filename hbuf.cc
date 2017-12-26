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
    if (hbuf_map[buf].empty()) {
	// all zones in this hbuf has already been cleaned
	// during the cleaning of other hbuffers
	// just reset the wp.
	disk->resetWritePointer(buf);
	return;
    }
    // printf("cleaning hbuf %u wp(%lx) containing %lu zones:", buf,
    // 	   disk->getWritePointer(buf), hbuf_map[buf].size());
    for (auto p: hbuf_map[buf]) {
	zone_t z = p.first;
	size_t data_size = p.second;
	Stats::getStats()->countHBuf(z, data_size);
    	// if (!z) printf("[%u=>%.2fMB](", z, data_size * 1.0 / (1024 * 1024));
	for (zone_t nbr_buf: zone_hbuf_map[z]) {
	    if (nbr_buf == buf) continue; // skip the current hbuf.
	    // if (!z) printf("hbuf%u %u->%.2fMB, ", nbr_buf, z,
	    // 	   hbuf_map[nbr_buf][z] * 1.0 / (1024 * 1024));
	    Stats::getStats()->countHBuf(z, hbuf_map[nbr_buf][z]);
	    hbuf_map[nbr_buf].erase(z);
	}
	// printf(")");
    }
    //    printf("\n");
    Stats::getStats()->countZoneClean(hbuf_map[buf].size());
    hbuf_map[buf].clear();
    disk->resetWritePointer(buf);
    // printf("cleaned  hbuf %u wp(%lx) size(%lu)\n", buf,
    // 	   disk->getWritePointer(buf), hbuf_map[buf].size());
}

void HBuf::writeToHBuf(ioreq req, zone_t buf){
    assert(buf < hbuf_num);
    if (disk->getWritePointer(buf) + req.len >= (buf + 1) * ZONE_SIZE)
	cleanHBuf(buf);
    zone_t zone = req.off / ZONE_SIZE;
    hbuf_map[buf][zone] += req.len;
    zone_hbuf_map[zone].insert(buf);

    req.off = disk->getWritePointer(buf);
    disk->write(req);
}

bool HBuf::checkHomeZoneSeq(ioreq req) {
    loff_t wp = disk->getWritePointer(UZONE2RAW(req.off / ZONE_SIZE));
    loff_t off = req.off + HBUF_NUM * ZONE_SIZE;
    return wp == off;
}

void HBuf::writeToHomeZone(ioreq req) {
    req.off += HBUF_NUM * ZONE_SIZE;
    disk->write(req);
}

// return value: the resultant logic offset. 
void HBuf::write(ioreq req){
    if (checkHomeZoneSeq(req)) {
	writeToHomeZone(req);
	return;
    }
    Stats::getStats()->countBytesWritten(req.len);
    zone_t z = req.off / ZONE_SIZE;
    Stats::getStats()->countOriginal(z, req.len);
    zone_t buf = policy->PickHBuf(this, req);
    // fall back to media cache
    if (buf == -1) {
	writeToHomeZone(req);
	return;
    }
    
    writeToHBuf(req, buf);
    // TODO insert poff mapping
    //    loff_t poff = writeToHBuf(req, p.PickHBuf(req));

}

void HBuf::read(ioreq req) {
    // TODO now ignore read request.
    //    printf("read req off=%lx, len=%lx\n", req.off, req.len);
    UNUSED(req); // supressing unused req warning.
    return;
}

void HBuf::hbufcleanup() {
    for (zone_t hbuf = 0; hbuf < hbuf_num; hbuf++) {
	// printf("final cleanup for hbuf: %u\n", hbuf);
	cleanHBuf(hbuf);
    }
}

void HBuf::cleanup() {
    disk->cleanup(); // media cache cleanup
    hbufcleanup(); // hbuf cleanup
}
