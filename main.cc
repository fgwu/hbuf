#include <iostream>
#include "global.h"
#include "disk.h"
#include "stats.h"
#include "tracereader.h"
#include "hbuf.h"

using namespace std;

int main(int argc, char** argv){
    if (argc != 2) {
	cout << argv[0] << " tracefile\n";
	return -1;
    }
    
    HBuf hbufdisk;
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
    return 0;
}
