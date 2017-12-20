#ifndef _POLICY_HASH_H_
#define _POLICY_HASH_H_

#include "disk.h"
#include "policy.h"

class Policy_Hash : public Policy{
 public:
    zone_t PickHBuf(HBuf*, ioreq);
};

#endif
