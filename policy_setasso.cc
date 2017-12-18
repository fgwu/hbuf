#include "global.h"
#include "policy_setasso.h"



zone_t Policy_SetAsso::PickHBuf(ioreq req) {
    zone_t zone = req.off / ZONE_SIZE;
    return zone % HBUF_NUM;
}
