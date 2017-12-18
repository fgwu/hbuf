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

long opt_alloc(vector<long>& hist, int bhuf_num);

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
	int size = stoi(val);

	getline(ss, val, ',');
	long responsetime = stol(val);

	if (!is_write) continue;

	// if (cnt_map.count(offset)) 
	//     cout << offset << " appeared " << cnt_map[offset] << "\n";

	cnt_map[offset]++;
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

    cout << opt_alloc(hist, 100) << "\n";
    
    return 0;
}
