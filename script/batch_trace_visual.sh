#!/bin/bash

trace_dir=/home/fwu/MSR-Cambridge/

for f in $(ls ${trace_dir}/*csv); do
    trace=$(echo $f | grep -Eo [[:alnum:]]+_[0-9])
    head -n 1000000 $f | grep "Write" > ${trace}_w.csv
    head -n 1000000 $f | grep "Read" > ${trace}_r.csv
    echo -n plotting ${trace} ...
    sed < trace_visual.gp.template "s/TRACE/${trace}/g" > trace_visual_${trace}.gp
    gnuplot trace_visual_${trace}.gp
    echo " done"
done


