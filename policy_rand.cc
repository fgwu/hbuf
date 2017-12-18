#include <cstdlib>
#include "global.h"
#include "policy_rand.h"



zone_t Policy_Rand::PickHBuf(ioreq req) {
    (void)req; // suppress unused warning
    return rand() % HBUF_NUM;
}
