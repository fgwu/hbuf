set terminal postscript eps color enhanced size 10,5 font "Times-Roman" 22
set output 'inj_rate_prn_1.eps'
set title "zone accu"
#set auto x
#set yrange [0:10]


plot 'prn_1_inj_rate_11.log' using 3:2 t 'zone 11' , \
'prn_1_inj_rate_12.log' using 3:2 t 'zone 12' , \
'prn_1_inj_rate_13.log' using 3:2 t 'zone 13' , \
'prn_1_inj_rate_812.log' using 3:2 t 'zone 812' , \
'prn_1_inj_rate_3.log' using 3:2 t 'zone 3' , \
'prn_1_inj_rate_4.log' using 3:2 t 'zone 4' , \
'prn_1_inj_rate_811.log' using 3:2 t 'zone 811' , \
'prn_1_inj_rate_944.log' using 3:2 t 'zone 944' , \
'prn_1_inj_rate_1012.log' using 3:2 t 'zone 1012', \
'prn_1_inj_rate_484.log' using 3:2 t 'zone 484'
