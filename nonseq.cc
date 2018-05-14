#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include "global.h"
#include "tracereader.h"

using namespace std;

#define HBUF_NUM 100
#define HBUF_SIZE (256L * 1024 * 1024 * HBUF_NUM)
#define BLK_SIZE 4096

int main(int argc, char** argv){
    if (argc < 2) {
	cout << argv[0] << " tracefile\n";
	return -1;
    }
    TraceReader tr(argv[1]);
    long lines = 0;
    ioreq req;
    map<zone_t, long> m;
    map<zone_t, unordered_set<long>> lba_set;
    while(tr.hasNext()) {
	if (++lines % 1000000 == 0)
	    cout << lines / 1000000 << " million lines\n";
	tr.next(req);
	if (!req.is_write) continue;
	zone_t z = req.off / ZONE_SIZE;
	m[z] += req.len;
	
	long lba_off = req.off % ZONE_SIZE / BLK_SIZE;
	long lba_num = req.len / BLK_SIZE;
	for (long i = 0; i < lba_num; i++) 
	    lba_set[z].insert(lba_off + i);

    }

    for (auto p: m)
	cout << "zone " << p.first << " " << p.second << " "
	     << lba_set[p.first].size() << "\n";
    
    return 0;
}
