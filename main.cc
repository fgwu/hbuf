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
    while(tr.hasNext()) {
	tr.next(req);
	//	printf("\ntrace req.off=%lx len=%lx\n", req.off, req.len);
	hbufdisk.write(req);
    }
    return 0;
}