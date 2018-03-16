#ifndef _POLICY_MULITLOG_H_
#define _POLICY_MULITLOG_H_

#include <unordered_map>
#include "disk.h"
#include "policy.h"
#include "hbuf.h"
class Policy_Multilog : public Policy{
 private:
    vector<pair<double, zone_t>>hist; // cnt and zone
    unordered_map<zone_t, pair<zone_t, zone_t>> zone_hbuf_map;
    unordered_map<zone_t, size_t> zone_inject_size;
    size_t max_win_size; // window size in terms of the incomeing traffic
    size_t win_size;
    size_t accu_size;

    long optAlloc();
    void recordReq(ioreq);
    double calcDiff(vector<pair<double, zone_t>> hist_old,
		    vector<pair<double, zone_t>> hist_new);
    
 public:
    Policy_Multilog(size_t);
    ~Policy_Multilog();
    zone_t PickHBuf(HBuf*, ioreq);
};

#endif
