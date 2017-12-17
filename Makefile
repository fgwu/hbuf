<<<<<<< HEAD
all: stat stat_zone

stat: stat.cc opt.cc opt.h
	g++ -std=c++11 -g stat.cc  opt.cc  -o stat

stat_zone: stat_zone.cc opt.cc opt.h
	g++ -std=c++11 -g stat_zone.cc  opt.cc  -o stat_zone

=======
all: sim opt

sim: stat.cc opt.cc opt.h
	g++ -std=c++11 -g stat.cc  opt.cc  -o stat

>>>>>>> 43c251f0ef99844830465cf53bcdae5557d50136
#opt: opt.cc  opt.h
#	g++ -std=c++11 -g opt.cc -o opt

clean:
<<<<<<< HEAD
	rm stat
=======
	rm opt stat
>>>>>>> 43c251f0ef99844830465cf53bcdae5557d50136
