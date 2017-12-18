#ifndef _POLICY_H_
#define _POLICY_H_

#include "disk.h"

class Policy {
 public:
    virtual ~Policy() {};
    virtual zone_t PickHBuf(ioreq) = 0;
};

#endif
