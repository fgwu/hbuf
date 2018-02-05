#ifndef _POLICY_H_
#define _POLICY_H_

#include <unordered_map>
#include "disk.h"

class HBuf;

enum policy_t {
    MEDIA,  // media cache
    SETASSO, // set associative
    RAND, // random
    HASH, // hash
    SLIDING, // sliding window
    SINGLELOG, // single log
    MULTILOG,
    HWINDOW
}; 

class Policy {
 public:
    virtual ~Policy() {};
    virtual zone_t PickHBuf(HBuf*, ioreq) = 0;
};

#endif
