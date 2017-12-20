#ifndef _POLICY_SINGLELOG_H_
#define _POLICY_SINGLELOG_H_

#include <unordered_map>
#include "disk.h"
#include "policy.h"
#include "hbuf.h"
class Policy_Singlelog : public Policy{
 private:
    long hbuf_accu_cnt; // accumulate cnt
 public:
    Policy_Singlelog();
    ~Policy_Singlelog();
    zone_t PickHBuf(HBuf*, ioreq);
};

#endif
