#include <cstdlib>
#include "global.h"
#include "policy_rand.h"

zone_t Policy_Rand::PickHBuf(HBuf* hbuf, ioreq req) {
    UNUSED(hbuf);
    UNUSED(req); // suppress unused warning
    return rand() % HBUF_NUM;
}
