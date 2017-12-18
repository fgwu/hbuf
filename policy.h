#ifndef _POLICY_H_
#define _POLICY_H_

#include "disk.h"

class Policy {
 private:
    static Policy policy;
 public:
    static Policy GetPolicy();
    zone_t PickHBuf(ioreq);
};

#endif
