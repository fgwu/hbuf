#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_


typedef long poff_t; // physical offset in bytes
typedef long loff_t; // logical offset in bytes
typedef long plen_t; // physical len in bytes
typedef long llen_t; // logical len in bytes

typedef int zone_t; // zone idx, number, etc...

#define HBUF_NUM 10
#define TOTAL_ZONE_NUM 30000
#define ZONE_SIZE (256*1024*1024)
#endif
