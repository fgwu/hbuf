#include "global.h"
#include "policy_hash.h"

zone_t Policy_Hash::PickHBuf(HBuf* hbuf, ioreq req) {
    UNUSED(hbuf);
    zone_t zone = req.off / ZONE_SIZE;
    // Knuth Method for Hash
    return zone * (zone + 3) % HBUF_NUM;
}
