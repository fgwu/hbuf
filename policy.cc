#include "global.h"
#include "policy.h"
#include "disk.h"


Policy Policy::GetPolicy() {
    return policy;
}

zone_t Policy::PickHBuf(ioreq req) {
    zone_t zone = req.off / ZONE_SIZE;
    return zone % HBUF_NUM;
}
