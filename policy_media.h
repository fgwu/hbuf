#ifndef _POLICY_MEDIA_H_
#define _POLICY_MEDIA_H_

#include "disk.h"
#include "policy.h"

class Policy_Media : public Policy{
 public:
    zone_t PickHBuf(HBuf*, ioreq);
};

#endif
