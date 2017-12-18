#include <iostream>
#include "global.h"
#include "disk.h"
#include "stats.h"
#include "tracereader.h"
using namespace std;

int main(){
	Disk d;
	Stats stats;
	cout << "hello world\n";
	TraceReader tr("proj_2_short.csv");
	ioreq req;
	while(tr.hasNext()) {
		tr.next(req);
		cout << req.off << " " << req.len << "\n";
		d.write(req);
	}
	return 0;
}
