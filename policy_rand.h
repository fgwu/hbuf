#ifndef _POLICY_RAND_H_
#define _POLICY_RAND_H_

#include "disk.h"
#include "policy.h"

class Policy_Rand : public Policy{
 public:
    zone_t PickHBuf(HBuf*, ioreq);
};

#endif
