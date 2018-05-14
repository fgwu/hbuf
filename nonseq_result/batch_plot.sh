#!/bin/bash


for f in $(ls *log); do
    trace=$(echo $f | grep -Eo [[:alnum:]]+_[0-9])
    echo -n plotting ${trace} ...
    sort ${trace}.log -k 3 -gr > ${trace}_sorted.csv
    sed < nonseq.gp.template "s/TRACE/${trace}/g" > ${trace}.gp
    gnuplot ${trace}.gp
    echo " done"
done


