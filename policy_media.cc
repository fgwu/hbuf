#include <cstdlib>
#include "global.h"
#include "policy_media.h"

zone_t Policy_Media::PickHBuf(HBuf* hbuf, ioreq req) {
    UNUSED(hbuf);
    UNUSED(req); // suppress unused warning
    // indicating no hbuf is selected. just write to media cache.
    return -1;
}
