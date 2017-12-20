#ifndef _POLICY_H_
#define _POLICY_H_

#include <unordered_map>
#include "disk.h"

class HBuf;

enum policy_t {
    SETASSO,
    RAND,
    HASH,
    SLIDING,
    SINGLELOG
}; 

class Policy {
 public:
    virtual ~Policy() {};
    virtual zone_t PickHBuf(HBuf*, ioreq) = 0;
};

#endif
