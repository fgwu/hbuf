#include <iostream>
#include "global.h"
#include "tracereader.h"
#include "rate_stats.h"

using namespace std;

int main(int argc, char** argv){
    if (argc != 3) {
	cout << argv[0] <<" tracefile window_size\n";
	return -1;
    }

    TraceReader tr(argv[1]);
    RateStats rs(atoi(argv[2]));
    ioreq req;
    int lines = 0;
    while(tr.hasNext()) {
	if (++lines % 1000000 == 0)
	    cout << lines / 1000000 << " million lines\n";
	tr.next(req);
	if(!req.is_write)
	    continue;
	rs.recordReq(req);
	// if (rs.recordReq(req))
	//     cout << "Phase Change Detected!\n";
    }
    return 0;
}
