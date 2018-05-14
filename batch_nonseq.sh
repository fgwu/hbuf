#!/bin/bash

nonseq_dir=nonseq_result

rm -rf tmp.log
for f in $(ls ~/MSR-Cambridge/*csv); do
    trace=$(echo $f | grep -Eo [[:alnum:]]+_[0-9])
#    echo $trace " " 
    outfile=${nonseq_dir}/${trace}.log
    :> ${outfile}
    ./nonseq $f | grep zone | awk '{print $2, $3, $4}' > ${outfile}
    wc -l ${outfile}
done

