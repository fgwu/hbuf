#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include "global.h"
#include "tracereader.h"

using namespace std;

#define HBUF_NUM 100
#define HBUF_SIZE (256L * 1024 * 1024 * HBUF_NUM)

int main(int argc, char** argv){
    if (argc < 2) {
	cout << argv[0] << " tracefile\n";
	return -1;
    }
    TraceReader tr(argv[1]);
    long lines = 0;
    ioreq req;
    map<zone_t, long> m;
    while(tr.hasNext()) {
	if (++lines % 1000000 == 0)
	    cout << lines / 1000000 << " million lines\n";
	tr.next(req);
	if (!req.is_write) continue;
	zone_t z = req.off / ZONE_SIZE;
	m[z] += req.len;
    }

    long sum = 0, sum_sqrt = 0;
    vector<double> vec, vec_sqrt;
    vector<zone_t> vec_zone_id;

    vector<pair<zone_t, long>> map_vec;
    for (auto p: m) 
	map_vec.push_back(p);

    sort(map_vec.begin(), map_vec.end(),
	 [](const pair<zone_t, long> &a, const pair<zone_t, long> &b){
	     return a.second < b.second;
	});
    
    for (auto p: map_vec) {
	cout << p.first << "," << p.second << "\n";
	vec.push_back(p.second);
	vec_sqrt.push_back(sqrt(p.second));
	vec_zone_id.push_back(p.first);
	sum += p.second;
	sum_sqrt += sqrt(p.second);
    }

    long single_cost = 0, multi_cost = 0;
    for (unsigned int i = 0; i < m.size(); i++) {
	double s = HBUF_SIZE * vec[i] / sum;
	single_cost += (int)(vec[i] / s) + 1;

	double s_sqrt = HBUF_SIZE * vec_sqrt[i] / sum_sqrt;
	multi_cost += (int)(vec[i] / s_sqrt) + 1;

	// cout << "zone:" << vec_zone_id[i]
	//      << " single=" << s / ZONE_SIZE << " " << (int)(vec[i] / s) + 1
	//      << " multi=" << s_sqrt / ZONE_SIZE
	//      << " " <<  (int)(vec[i] / s_sqrt) + 1 << "\n";

	printf("zone %4d | media %4.1f %3d | sqrt %4.1f %3d\n",
	       vec_zone_id[i], s / ZONE_SIZE, (int)(vec[i] / s) + 1,
	       s_sqrt / ZONE_SIZE, (int)(vec[i] / s_sqrt) + 1);
    }

    printf("#zone %3lu |            %3ld |           %3ld",
	   m.size(), single_cost, multi_cost);

    if (single_cost > multi_cost)
	cout << "   win!!";
    else if (single_cost ==  multi_cost)
	cout << " -------- ";

    if (sum < HBUF_SIZE)
	cout << " small ";
    cout << "\n";
    
    return 0;
}
