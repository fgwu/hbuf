#include "global.h"
#include "policy_setasso.h"



zone_t Policy_SetAsso::PickHBuf(HBuf* hbuf, ioreq req) {
    UNUSED(hbuf);
    zone_t zone = req.off / ZONE_SIZE;
    return zone % HBUF_NUM;
}
