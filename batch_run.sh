#!/bin/bash

#logdir=./logfiles_all
outfile=run.log
:> $outfile

for trace in $(cat estimate.log   | grep -v small |cut -d " " -f 1); do
#    trace=$(echo $f | grep -Eo [[:alnum:]]+_[0-9])
    echo -n $trace " " | tee -a $outfile
    f=./tracefile_ssd/${trace}.csv
    ./sim $f media | grep zone_cleaned | cut -d " " -f 2 >> $outfile
done

