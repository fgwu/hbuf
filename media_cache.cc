#include <queue>
#include <unordered_map>
#include <queue>
#include <cassert>
#include "media_cache.h"
#include "disk.h"
#include "stats.h"

//#define LIMITED_MEDIA_CACHE_MAPPING

Media_Cache::Media_Cache() { serial_no = 0; }

Media_Cache::~Media_Cache() {}

loff_t Media_Cache::write(ioreq req) {
    loff_t start = mq.empty()? 0 : mq.back().end;
    if (start + req.len > MEDIA_CACHE_SIZE) start = 0;
    loff_t end = start + req.len;


    #ifdef LIMITED_MEDIA_CACHE_MAPPING
    // ensure that both the media cache capacity and the mapping are not
    // excceeded.
    while (mq.size() >= MEDIA_CACHE_MAPPING_SIZE ||
    	   (mq.size() && end > mq.front().start && start < mq.front().end)) 
    	clean();

    #else
    while (mq.size() && end > mq.front().start && start < mq.front().end)
	clean();
    #endif
    
    zone_t zone = req.off / ZONE_SIZE;
    mq.push(media_cache_entry(++serial_no, zone, start, end));
    
    return start;
}

void Media_Cache::clean() {
    assert(mq.size());
    media_cache_entry me = mq.front();
    mq.pop();

    // here me.zone is the disk zone to which the data belongs.
    // we transfer it back to the user zone_id by substracting the HBUF_NUM.
    Stats::getStats()->countMedia(me.zone - HBUF_NUM, me.end - me.start);

    // if current entry is already invalidated.
    if (valid_sn.count(me.zone) && me.serial_no < valid_sn[me.zone]) {
	// printf("media cache skipping: %5d (%7lld <  %7lld)\n",
	//        me.zone, me.serial_no, valid_sn[me.zone]);
	return;
    }  else {
	printf("media cache cleaning: %5d (%7lld >= %7lld)\n",
	       me.zone, me.serial_no, valid_sn[me.zone]);
    }
    Stats::getStats()->countPerZoneClean(me.zone - HBUF_NUM); // me.zone is cleaned
    Stats::getStats()->countZoneClean(1); // one more zone is cleaned
    valid_sn[me.zone] = serial_no;
}

void Media_Cache::cleanup() {
    printf("media cache cleaning %lu entries ...", mq.size());
    while(mq.size()) 
	clean();
    printf(" done\n");
}
