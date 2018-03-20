#ifndef _POLICY_SQRT_H_
#define _POLICY_SQRT_H_

#include <unordered_map>
#include "disk.h"
#include "policy.h"
#include "hbuf.h"
class Policy_Sqrt : public Policy{
 private:
    vector<pair<double, zone_t>>hist; // cnt and zone
    unordered_map<zone_t, pair<zone_t, zone_t>> zone_hbuf_map;
    unordered_map<zone_t, zone_t> hbuf_cursor;
    unordered_map<zone_t, long> zone_inject_size;
    long max_win_size; // window size in terms of the incomeing traffic
    long win_size;
    long accu_size;

    long optAlloc();
    void recordReq(ioreq);

    
 public:
    Policy_Sqrt();
    ~Policy_Sqrt();
    zone_t PickHBuf(HBuf*, ioreq);
};

#endif
