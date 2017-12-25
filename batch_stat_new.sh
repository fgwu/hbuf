#!/bin/bash

logdir=./logfiles_all
outfile=stat.log
:> $outfile

for f in $(ls tracefile_ssd/*csv); do
    trace=$(echo $f | grep -Eo [[:alnum:]]+_[0-9])
    echo -n $trace " " | tee -a $outfile
    for p in setasso rand sliding singlelog media; do
	#	echo -n trace $trace policy $p
	:> ${logdir}/${trace}_${p}.log
	./sim $f $p > ${logdir}/${trace}_${p}.log
	overhead=$(cat ${logdir}/${trace}_${p}.log | grep overhead | awk '{print $2}')
	echo -n $overhead "," | tee -a $outfile
    done
    echo | tee -a $outfile
done

