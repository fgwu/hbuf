#include <queue>
#include <unordered_map>
#include <queue>
#include <cassert>
#include "media_cache.h"
#include "disk.h"
#include "stats.h"

Media_Cache::Media_Cache() { serial_no = 0; }

Media_Cache::~Media_Cache() {}

loff_t Media_Cache::write(ioreq req) {
    if (mq.size()) {
	loff_t head = mq.back().end;
	loff_t tail = mq.front().start;
	bool is_full = head > tail ?
	    head + (loff_t)req.len > (loff_t)MEDIA_CACHE_SIZE + tail :
	    head + (loff_t)req.len > tail;

	while(mq.size() && is_full) {
	    clean();
	    head = mq.back().end;
	    tail = mq.front().start;
	    is_full = head > tail ?
		head + (loff_t)req.len > (loff_t)MEDIA_CACHE_SIZE + tail :
		head + (loff_t)req.len > tail;
	}
    }

    loff_t off = mq.empty() ? 0 : mq.back().end;
    zone_t zone = req.off / ZONE_SIZE;
    mq.push(media_cache_entry(++serial_no, zone, off, (off + req.len) % MEDIA_CACHE_SIZE));
    // printf("media cache writing : %5d (%7lld) off=%ld len=%lu\n",
    // 	   zone, serial_no, off, req.len);
    return off;
}

void Media_Cache::clean() {
    assert(mq.size());
    media_cache_entry me = mq.front();
    mq.pop();

    // if current entry is already invalidated.
    if (valid_sn.count(me.zone) && me.serial_no < valid_sn[me.zone]) {
	// printf("media cache skipping: %5d (%7lld <  %7lld)\n",
	//        me.zone, me.serial_no, valid_sn[me.zone]);
	return;
    }  else
       	printf("media cache cleaning: %5d (%7lld >= %7lld)\n",
      	       me.zone, me.serial_no, valid_sn[me.zone]);
    Stats::getStats()->countZoneClean(1); // one more zone is cleaned
    valid_sn[me.zone] = serial_no;
}
