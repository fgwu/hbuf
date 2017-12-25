#ifndef _HBUF_H_
#define _HBUF_H_

#include <vector>
#include <unordered_set>
#include "fakedisk.h"
#include "stats.h"
#include "policy.h"

using namespace std;

/* This class is the disk wrapped by hbuf
 */
class HBuf {
private:
    zone_t hbuf_num;
    zone_t uzone_num;
    Policy *policy;
    // bhuf id => data's home zone collection => data size.
    vector<unordered_map<zone_t, size_t>> hbuf_map;
    // reverse map: zone=>hbuffers that has its data
    unordered_map<zone_t, unordered_set<zone_t>> zone_hbuf_map;
    void cleanHBuf(zone_t buf);
    loff_t writeToHBuf(ioreq req, zone_t zone);
    void hbufcleanup(); // clean up all zones remaining in hbuf
 public:
    Disk *disk;
    HBuf(Policy* p);
    ~HBuf();
    void write(ioreq req);
    void read(ioreq req);
    void cleanup();
};


#endif
