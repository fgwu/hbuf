set terminal postscript eps color enhanced size 10,5 font "Times-Roman" 22
set output 'inj_rate_proj_0.eps'
set title "zone accu"
#set auto x
#set yrange [0:1000]
#set xrange [0:1000]

set key top left

#plot 'proj_0_inj_rate_11.log' using 3:2 t 'zone 11' , \
#'proj_0_inj_rate_12.log' using 3:2 t 'zone 12' , \
#'proj_0_inj_rate_14.log' using 3:2 t 'zone 14'

plot 'proj_0_inj_rate_11.log' using 3:2 t 'zone 11' , \
'proj_0_inj_rate_63.log' using 3:2 t 'zone 63' , \
'proj_0_inj_rate_7.log' using 3:2 t 'zone 7' , \
'proj_0_inj_rate_16.log' using 3:2 t 'zone 16' , \
'proj_0_inj_rate_61.log' using 3:2 t 'zone 61' , \
'proj_0_inj_rate_12.log' using 3:2 t 'zone 12' , \
#'proj_0_inj_rate_5.log' using 3:2 t 'zone 5' , \
#'proj_0_inj_rate_32.log' using 3:2 t 'zone 32' , \
#'proj_0_inj_rate_56.log' using 3:2 t 'zone 56', \
#'proj_0_inj_rate_14.log' using 3:2 t 'zone 14'
