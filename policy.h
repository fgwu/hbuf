#ifndef _POLICY_HPP_
#define _POLICY_HPP_

#include "disk.h"

class Policy {
private:
	static Policy policy;
public:
	static Policy GetPolicy();
    	zone_t PickHBuf(ioreq);
};

#endif
