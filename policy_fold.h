#ifndef _POLICY_FOLD_H_
#define _POLICY_FOLD_H_

#include <unordered_map>
#include "disk.h"
#include "policy.h"
#include "hbuf.h"
class Policy_Fold : public Policy{
 private:
    vector<pair<long, zone_t>>hist; // cnt and zone
    unordered_map<zone_t, pair<zone_t, zone_t>> zone_hbuf_map;
    unordered_map<zone_t, size_t> zone_inject_size;
    long full_cnt;
    bool folded;

    void calc();
    void fold();
    long optAlloc();
    void recordReq(ioreq);
    
 public:
    Policy_Fold();
    ~Policy_Fold();
    zone_t PickHBuf(HBuf*, ioreq);
};

#endif
