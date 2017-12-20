#ifndef _POLICY_SLIDING_H_
#define _POLICY_SLIDING_H_

#include <unordered_map>
#include "disk.h"
#include "policy.h"
class Policy_Sliding : public Policy{
 private:
    vector<pair<long, zone_t>>hist; // cnt and zone
    unordered_map<zone_t, zone_t> zone_hbuf_map;
    unordered_map<zone_t, size_t> zone_inject_size;
    size_t window_size; // window size in terms of the incomeing traffic
    size_t accu_size;


    long optAlloc();
    void recordReq(ioreq);
    
 public:
    Policy_Sliding(size_t);
    ~Policy_Sliding();
    zone_t PickHBuf(ioreq);
};

#endif
