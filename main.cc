#include <iostream>
#include <cassert>
#include "global.h"
#include "disk.h"
#include "stats.h"
#include "tracereader.h"
#include "hbuf.h"
#include "policy.h"
#include "policy_setasso.h"
#include "policy_rand.h"
#include "policy_hash.h"

using namespace std;

unordered_map<string, policy_t> policy_to_enum = {
    {"setasso", SETASSO}, {"rand", RAND}, {"hash", HASH}};

int main(int argc, char** argv){
    if (argc < 2) {
	cout << argv[0] << " tracefile [policy]\n";
	cout << "supported policies: setasso(default) rand hash\n";
	return -1;
    }

    string policy_name;
    if (argc == 2)
	policy_name = "setasso";
    else
	policy_name = argv[2];

    if (!policy_to_enum.count(policy_name)) {
	cout << "supported policies: setasso(default) rand hash\n";
	return -1;
    }

    Policy * p;
    switch(policy_to_enum[policy_name]) {
    case SETASSO:
	p = new Policy_SetAsso();
	break;
    case RAND:
	p = new Policy_Rand();
	break;
    case HASH:
	p = new Policy_Hash();
	break;
    default:
	assert(0);
    }

    HBuf hbufdisk(p);
    TraceReader tr(argv[1]);
    ioreq req;
    int lines = 0;
    while(tr.hasNext()) {
	if (++lines % 1000000 == 0)
	    cout << lines / 1000000 << " million lines\n";

	tr.next(req);
	//	printf("\ntrace req.off=%lx len=%lx\n", req.off, req.len);
	if(req.is_write)
	    hbufdisk.write(req);
	else
	    hbufdisk.read(req);
    }
    Stats::getStats()->report();
    delete p;
    return 0;
}
