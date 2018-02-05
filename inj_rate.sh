#!/bin/bash

trace=proj_0

cat tracefile_ssd/${trace}.csv  | awk -F',' '{if ($4 == "Write") {print int($5 / 256 / 1024 / 1024);}}' | sort -g |cut -d' ' -f 1| uniq -c | sort -g -r|tee ${trace}_inj.csv

for zone in $(head -n 10 ${trace}_inj.csv | awk '{print $2;}'); do
    echo zone $zone
    cat tracefile_ssd/${trace}.csv  | awk -F',' -v zone=$zone '{z = int($5/256/1024/1024); if ($4 == "Write"&& z==zone) {print z, zaccu/1024/1024, accu/1024/1024; zaccu+=$6} if ($4 == "Write") accu+= $6}'  > ${trace}_inj_rate_${zone}.log

done


#for zone in $(head -n 600 ${trace}_inj.csv | tail -n 10 | awk '{print $2;}'); do
#    echo zone $zone
#    cat tracefile_ssd/${trace}.csv  | awk -F',' -v zone=$zone '{z = int($5/256/1024/1024); if ($4 == "Write"&& z==zone) {print z, zaccu/1024/1024, accu/1024/1024; zaccu+=$6} if ($4 == "Write") accu+= $6}'  > ${trace}_inj_rate_${zone}.log

#done

