#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <cstdlib>

typedef int zone_t; // zone idx, number, etc...

const zone_t HBUF_NUM (100);
const zone_t TOTAL_ZONE_NUM (30000);
const size_t ZONE_SIZE (256*1024*1024);
const size_t MEDIA_CACHE_SIZE (100 * ZONE_SIZE); // 25.6 GB

#define UNUSED(var) do { (void)(var); } while(0);

#endif
