#include <climits>
#include <algorithm>
#include "global.h"
#include "policy_singlelog.h"
#include "hbuf.h"

Policy_Singlelog::Policy_Singlelog() {
    hbuf_accu_cnt = 0;
}

Policy_Singlelog::~Policy_Singlelog() {}

zone_t Policy_Singlelog::PickHBuf(HBuf *hbuf, ioreq req) {
    zone_t cand = hbuf_accu_cnt % HBUF_NUM;
    if (hbuf->disk->getWritePointer(cand) + req.len >=
	(cand + 1) * ZONE_SIZE) {
	return (++hbuf_accu_cnt) % HBUF_NUM;	
    }
    return cand;
}
