all: stat stat_zone

stat: stat.cc opt.cc opt.h
	g++ -std=c++11 -g stat.cc  opt.cc  -o stat

stat_zone: stat_zone.cc opt.cc opt.h
	g++ -std=c++11 -g stat_zone.cc  opt.cc  -o stat_zone

#opt: opt.cc  opt.h
#	g++ -std=c++11 -g opt.cc -o opt

clean:
	rm stat
