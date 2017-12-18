#ifndef _DISK_HPP_
#define _DISK_HPP_

#include <vector>

using namespace std;

struct ioreq {
    loff_t off; // in bytes
    llen_t len;
    ioreq(): off(0), len(0) {}
    ioreq(loff_t s,  llen_t l): off(s), len(l) {}
};

/* This class is the simulated disk. Will later be replaced by the
 * real disk. 
 */
class Disk {
private:
    zone_t total_zone_num;
    zone_t hbuf_num;
    vector<poff_t> wp;
    vector<poff_t> hwp; /*hbuf write pointers */
public:
    Disk();
    ~Disk();
    poff_t write(ioreq);
    poff_t writeToZone(ioreq req, zone_t zone);
    poff_t writeToHBuf(ioreq req, zone_t zone);
};


#endif
