#ifndef _HBUF_H_
#define _HBUF_H_

#include <vector>
#include <unordered_set>
#include "fakedisk.h"
#include "stats.h"

using namespace std;

/* This class is the disk wrapped by hbuf
 */
class HBuf {
private:
    zone_t hbuf_num;
    zone_t uzone_num;
    FakeDisk *disk;
    Stats *stats;
    // bhuf id => data's home zone collection.
    vector<unordered_set<zone_t>> hbuf_map; 
    void cleanHBuf(zone_t buf);
    loff_t writeToHBuf(ioreq req, zone_t zone);
 public:
    HBuf();
    ~HBuf();
    void write(ioreq req);
    void read(ioreq req);
};


#endif
