#!/usr/bin/python

import random
f = open("synth_trace.csv", 'w')
N = 1000 * 1024 * 1024 * 1024  / (4 * 1024)

zone_blks = 256 * 1024 * 1024 / (4 * 1024)

hot_start = 64 * 256 * 1024 * 1024
cold_start = 65 * 256 * 1024 * 1024

for x in range(0, N):
    zone_off = random.randint(0, zone_blks - 1) * 4096
    r = random.randint(0, 11)
    if (x % (N/100) == 0 ):
        print x / (N / 100), r
#    print r
    if (r == 0):
        offset = cold_start + zone_off
    else:
        offset = hot_start + zone_off

    f.write("0,wdev,3,Write," + str(offset) + ",4096,0\n");


f.close()
