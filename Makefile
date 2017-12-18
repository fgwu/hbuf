CXX = g++
CXXFLAGS = -Wall -g -std=c++11
OBJ = disk.o main.o \
	stats.o tracereader.o policy.o
PROG = sim

$(PROG): $(OBJ)
	g++ -o $(PROG) $(OBJ)

$(OBJ): global.h policy.h

tracereader.o: disk.h


.PHONY: clean
clean:
	-rm $(PROG) $(OBJ)
