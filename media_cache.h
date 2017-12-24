#ifndef _MEDIA_CACHE_H_
#define _MEDIA_CACHE_H_

#include <queue>
#include <unordered_map>
#include "global.h"
#include "disk.h"

using namespace std;
struct media_cache_entry {
    long long serial_no; // serial number
    zone_t zone;
    loff_t start;
    loff_t end;

    media_cache_entry(long long sn, zone_t z, loff_t s, loff_t e): serial_no(sn), zone(z), start(s), end(e) {}
};

class Media_Cache {
 private:
    long long serial_no;
    queue<media_cache_entry> mq; // media_cache entry queue
    unordered_map<zone_t, long long> valid_sn; // last clean up to
    
    void clean();
 public:
    Media_Cache();
    ~Media_Cache();
    loff_t write(ioreq req);
};

#endif
