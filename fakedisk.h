#ifndef _FAKEDISK_H_
#define _FAKEDISK_H_

#include <vector>
#include "disk.h"
#include "media_cache.h"

using namespace std;

/* This class is the simulated HM disk. Will later be replaced by the
 * real disk. 
 */
class FakeDisk: public Disk{
private:
    zone_t total_zone_num;
    vector<loff_t> wp;
    Media_Cache mc;
public:
    FakeDisk();
    ~FakeDisk();
    loff_t write(ioreq req);
    size_t read(ioreq req);
    bool resetWritePointer(zone_t zone);
    loff_t getWritePointer(zone_t zone);
    zone_t getZoneTotalNum();
    void cleanup();
};


#endif

