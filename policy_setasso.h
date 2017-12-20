#ifndef _POLICY_SETASSO_H_
#define _POLICY_SETASSO_H_

#include "disk.h"
#include "policy.h"

class Policy_SetAsso : public Policy{
 public:
    zone_t PickHBuf(HBuf*, ioreq);
};

#endif
