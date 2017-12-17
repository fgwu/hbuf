all: sim opt

sim: stat.cc opt.cc opt.h
	g++ -std=c++11 -g stat.cc  opt.cc  -o stat

#opt: opt.cc  opt.h
#	g++ -std=c++11 -g opt.cc -o opt

clean:
	rm opt stat
