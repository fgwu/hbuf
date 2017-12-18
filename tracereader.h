#ifndef _TRACEREADER_HPP_
#define _TRACEREADER_HPP_

#include <fstream>

class TraceReader {
    string tracefile;
    ifstream filestream;
    string trace_line;
public:
    TraceReader(string);
    ~TraceReader();
    void next(ioreq&);
    bool hasNext();
};

#endif
