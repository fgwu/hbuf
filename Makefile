CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -Wextra -pedantic

OBJ =  main.o \
	stats.o tracereader.o fakedisk.o hbuf.o media_cache.o \
	policy_setasso.o policy_rand.o policy_hash.o policy_sliding.o \
	policy_singlelog.o policy_media.o policy_hwindow.o \
	policy_multilog.o policy_fold.o policy_sqrt.o

PROG = sim
ALL: $(PROG) analyze


$(PROG): $(OBJ)
	$(CXX) -o $(PROG) $(OBJ) $(CXXFLAGS)

analyze: trace_analyze.o tracereader.o
	g++ trace_analyze.cc tracereader.cc -o analyze $(CXXFLAGS)

$(OBJ): global.h policy.h disk.h stats.h fakedisk.h

policy_singlelog.o: policy_singlelog.h
policy_media.o: policy_media.h

.PHONY: clean
clean:
	-rm $(PROG) $(OBJ) analyze
