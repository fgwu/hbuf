#!/bin/bash

:>stat.log

for f in $(ls tracefile_ssd/*csv); do
    trace=$(echo $f | grep -Eo [[:alnum:]]+_[0-9])
    echo processing $trace ...
    ./stat_zone $f > $trace.log

    opt_total_data=$(cat $trace.log | grep opt_total_data | awk '{print $2}')
    opt_clean_data=$(cat $trace.log | grep opt_clean_data | awk '{print $2}')
    opt_clean_overhead=$(cat $trace.log | grep opt_clean_overhead | awk '{print $2}')

    rand_total_data=$(cat $trace.log | grep rand_total_data | awk '{print $2}')
    rand_clean_data=$(cat $trace.log | grep rand_clean_data | awk '{print $2}')
    rand_clean_overhead=$(cat $trace.log | grep rand_clean_overhead | awk '{print $2}')

    
    echo $trace $opt_total_data $opt_clean_data $opt_clean_overhead $rand_total_data $rand_clean_data $rand_clean_overhead | tee -a stat.log
done

