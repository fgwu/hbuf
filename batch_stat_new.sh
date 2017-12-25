#!/bin/bash

logdir=./logfiles_all
:> stat.log

for f in $(ls tracefile_ssd/*csv); do
    trace=$(echo $f | grep -Eo [[:alnum:]]+_[0-9])
    echo -n $trace " " | tee -a stat.log
    for p in setasso rand sliding singlelog media; do
	#	echo -n trace $trace policy $p
	:> ${logdir}/${trace}_${p}.log
	./sim $f $p > ${logdir}/${trace}_${p}.log
	overhead=$(cat ${logdir}/${trace}_${p}.log | grep overhead | awk '{print $2}')
	echo -n $overhead "," | tee -a stat.log
    done
    echo | tee -a stat.log
done

