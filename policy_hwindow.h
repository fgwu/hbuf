#ifndef _POLICY_HWINDOW_H_
#define _POLICY_HWINDOW_H_

#include <unordered_map>
#include "disk.h"
#include "policy.h"
#include "hbuf.h"

const long int SEG_SIZE = 1024 * 1024 * 1024;
const double ALPHA = 0.9;
const double BETA = 0.3;

class Policy_Hwindow : public Policy{
 private:
    vector<pair<double, zone_t>>hist; // estimated stat for future
    unordered_map<zone_t, pair<zone_t, zone_t>> zone_hbuf_map;
    unordered_map<zone_t, double> stat_seg;
    unordered_map<zone_t, double> stat_est;
    size_t accu_size;

    double alpha; // fading factor; the higher, the more weight of history
    double beta; // significant change threshold

    void optAllocItr(vector<vector<double>>&,
		     vector<vector<double>>&,
		     vector<vector<pair<long, long>>>&);
    double optAllocRec(vector<vector<double>>&,
		     vector<vector<double>>&,
		     vector<vector<pair<long, long>>>&,
		     long, long);
    long optAlloc();
    void recordReq(ioreq);

    double calcDiff(unordered_map<zone_t, double>,
		     unordered_map<zone_t, double>);
    
 public:
    Policy_Hwindow();
    ~Policy_Hwindow();
    zone_t PickHBuf(HBuf*, ioreq);
};

#endif
