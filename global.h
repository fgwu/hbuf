#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <cstdlib>

typedef int zone_t; // zone idx, number, etc...

const zone_t HBUF_NUM (100);
const zone_t TOTAL_ZONE_NUM (30000);
const size_t ZONE_SIZE (256*1024*1024);
const size_t MEDIA_CACHE_SIZE (100 * ZONE_SIZE); // 25.6 GB
const size_t MEDIA_CACHE_MAPPING_SIZE (185000);
const size_t BLK_SIZE (512);

#define UNUSED(var) do { (void)(var); } while(0);
#define UZONE2RAW(uzone_id) (uzone_id + HBUF_NUM) // user zone to raw mapping
#define HZONE2RAW(hzone_id) (hzone_id) // hbuf zone to raw mapping
#endif
