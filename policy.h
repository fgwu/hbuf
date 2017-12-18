#ifndef _POLICY_H_
#define _POLICY_H_

#include <unordered_map>
#include "disk.h"

enum policy_t {
    SETASSO,
    RAND,
    HASH}; 

class Policy {
 public:
    virtual ~Policy() {};
    virtual zone_t PickHBuf(ioreq) = 0;
};

#endif
