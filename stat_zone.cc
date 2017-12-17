//#include "opt.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>


using namespace std;

#define ZONE_SIZE (256 * 1024 * 1024) // 256MB
#define BLK_SIZE (4 * 1024) // 4KB
//typedef unsigned long smr_off_t;
//typedef unsigned int smr_size_t;
//typedef unsigned int smr_zone_t;


long opt(vector<long>& hist, int bhuf_num);

int main(int argc, char** argv) {
    if (argc != 2) {
	printf("usage: %s tracefile\n", argv[0]);
	return -1;
    }
    unordered_map<long, long> cnt_map;
    ifstream f(argv[1]);
    string tmp;

    int lines = 0;
    while(getline(f, tmp)) {
	if (++lines % 1000000 == 0)
	    cout << lines / 1000000 << " million lines\n";
	
	stringstream ss(tmp);
	string val, hostname;
	getline(ss, val, ',');
	long ts = stol(val);

	getline(ss, hostname, ',');

	getline(ss, val, ',');
	int diskno = stoi(val);

	getline(ss, val, ',');
	bool is_write = val == "Write";

	getline(ss, val, ',');
	long offset = stol(val);

	getline(ss, val, ',');
	long size = stoi(val);

	getline(ss, val, ',');
	long responsetime = stol(val);

	if (!is_write) continue;

	// if (cnt_map.count(offset)) 
	//     cout << offset << " appeared " << cnt_map[offset] << "\n";

	int zone_id = offset / ZONE_SIZE;
	// truncate away from zero
	int num_blocks = (size + BLK_SIZE - 1) / BLK_SIZE; 
	cnt_map[zone_id]++;
    }

    cout << "pushing to vector\n";
    vector<long> hist;
    for (auto p: cnt_map) 
	if (p.second > 10)
	    hist.push_back(p.second);

    cout << "sorting vector\n";
    sort(hist.begin(), hist.end()/*, greater<long>()*/);
    int cnt = 0;
    for (auto a: hist) {
	if (a == 1)  
	    cnt++;
        else
	    cout << a << " ";
    }

    cout << "tail:" << cnt << "\n";
    cout << "Zone Covered:" << cnt_map.size() << "\n";

    cout << opt(hist, 100) << "\n";
    
    return 0;
}
