#ifndef _DISK_HPP_
#define _DISK_HPP_

#include <cstdlib>
#include <vector>

using namespace std;

struct ioreq {
    loff_t off; // in bytes
    size_t len;
    bool is_write;
    
    ioreq(): off(0), len(0), is_write(false) {}
    ioreq(loff_t s,  size_t l, bool w): off(s), len(l), is_write(w) {}
};

/* This class is the disk interface.
 */
class Disk {
public:
    virtual ~Disk() {};
    virtual loff_t write(ioreq req) = 0;
    virtual size_t read(ioreq req) = 0;
    virtual bool resetWritePointer(zone_t zone) = 0;
    virtual loff_t getWritePointer(zone_t zone) = 0;
    virtual zone_t getZoneTotalNum() = 0;
    virtual void cleanup() = 0; // cleanup media cache
};
#endif
