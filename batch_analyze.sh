#!/bin/bash

#logdir=./logfiles_all
outfile=analyze.log
:> $outfile

for f in $(ls tracefile_ssd/*csv); do
    trace=$(echo $f | grep -Eo [[:alnum:]]+_[0-9])
    echo -n $trace " " | tee -a $outfile

    median=$(./analyze $f | grep overall | cut -d ' ' -f6)
    
    echo $median | tee -a $outfile
done

