#!/bin/bash

#logdir=./logfiles_all
outfile=estimate.log
:> $outfile

for f in $(ls tracefile_ssd/*csv); do
    trace=$(echo $f | grep -Eo [[:alnum:]]+_[0-9])
    echo -n $trace " " | tee -a $outfile

    ./estimate $f | grep \# | tee -a $outfile
done

