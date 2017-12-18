all: stat stat_zone

stat: stat.cc opt_alloc.cc
	g++ -std=c++11 -g stat.cc  opt_alloc.cc  -o stat

stat_zone: stat_zone.cc opt_alloc.cc rand_alloc.cc
	g++ -std=c++11 -g stat_zone.cc  opt_alloc.cc rand_alloc.cc -o stat_zone

clean:
	rm stat stat_zone
