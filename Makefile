CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -Wextra -pedantic
OBJ =  main.o \
	stats.o tracereader.o fakedisk.o hbuf.o \
	policy_setasso.o policy_rand.o policy_hash.o policy_sliding.o \
	policy_singlelog.o
PROG = sim

$(PROG): $(OBJ)
	g++ -o $(PROG) $(OBJ)

$(OBJ): global.h policy.h disk.h stats.h fakedisk.h

policy_singlelog.o: policy_singlelog.h

.PHONY: clean
clean:
	-rm $(PROG) $(OBJ)
