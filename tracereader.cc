#include <iostream>
#include <string>
#include <sstream>
#include "global.h"
#include "disk.h"
#include "tracereader.h"

using namespace std;

TraceReader::TraceReader(string tf) {
    cout << "init tracereader " << tf << "\n";
    tracefile = tf;
    filestream.open(tf);
    if (filestream.fail()) {
	cout << "open " << tf <<  "failed!\n";
	exit(1);
    };
}

TraceReader::~TraceReader() {
    filestream.close();
}

bool TraceReader::hasNext() {
    if (!getline(filestream, trace_line)) return false;
    return true;
}

void TraceReader::next(ioreq& req) {
    string Timestamp,Hostname,DiskNumber,Type,Offset,Size,ResponseTime;
	
    stringstream ss(trace_line);

    /*	cout << "<" <<  s << ">\n";*/

    getline(ss, Timestamp, ',');
    getline(ss, Hostname, ',');
    getline(ss, DiskNumber, ',');
    getline(ss, Type, ',');
    getline(ss, Offset, ',');
    getline(ss, Size, ',');
    getline(ss, ResponseTime, ',');
    /*	cout << Timestamp << "\n";
	cout << Hostname << " ";
	cout << DiskNumber << " ";
	cout << Type << " ";
	cout << Offset << " ";
	cout << Size << " ";
	cout << ResponseTime << "\n";
    */
    req.off = stol(Offset);
    req.len = stol(Size);
    req.is_write = Type == "Write";
}
	
