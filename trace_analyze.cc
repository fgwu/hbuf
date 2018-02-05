#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include "global.h"
#include "tracereader.h"

using namespace std;

size_t find_percentile(vector<size_t>& tmp, double p) {
    int k = tmp.size() * p;
    auto it = tmp.begin() + k;
    nth_element(tmp.begin(), it, tmp.end());
    return *it;
}

int main(int argc, char** argv){
    if (argc < 2) {
	cout << argv[0] << " tracefile\n";
	return -1;
    }
    TraceReader tr(argv[1]);
    long lines = 0;
    ioreq req;
    size_t accu = 0;
    map<zone_t, vector<size_t>> m;
    while(tr.hasNext()) {
	if (++lines % 1000000 == 0)
	    cout << lines / 1000000 << " million lines\n";
	tr.next(req);
	if (!req.is_write) continue;
	zone_t z = req.off / ZONE_SIZE;
	m[z].push_back(accu);
	accu += req.len;
    }

    map<zone_t, size_t> res;
    int single_zone = 0;
    for (auto p: m) {
	if (p.second.size() <= 1) {
	    res[p.first] = -1;
	    single_zone++;
	    continue;
	}
	vector<size_t> tmp;
	for (size_t i = 1; i < p.second.size(); i++) 
	    tmp.push_back(p.second[i] - p.second[i - 1]);
	res[p.first] = find_percentile(tmp, 0.5);
    }

    vector<size_t> medians;
    for (auto p: res) {
	printf("zone %5d size %7lu median ", p.first, m[p.first].size());
	printf("%lu\n", p.second);
	medians.push_back(p.second);
	// if (m[p.first].size() <= 1)
	//     printf("MAX\n");
	// else
	//     printf("%lu\n", p.second);
    }

    printf("zone count %lu overall meidan %lu\n",
	   medians.size(),
	   find_percentile(medians, 0.5));
    return 0;
}
