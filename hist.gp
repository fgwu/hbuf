set terminal postscript eps color enhanced size 10,5 font "Times-Roman" 22
set output 'hist.eps'
set title "write amp due to hbuf cleaning"
set auto x
set yrange [0:50]
set style data histogram
set style histogram cluster gap 3
set style fill solid border -1
set boxwidth 0.9
set xtic rotate by -45 scale 0
set key outside
#set bmargin 10
#plot 'stat.log' using 2:xtic(1) t 'set asso', '' u 3:xtic(1) t 'rand', '' u 4:xtic(1) t 'sliding', '' u 5:xtic(1) t 'singlelog', '' u 6:xtic(1) t 'media'
plot 'stat.log' using 2:xtic(1) t 'set asso', '' u 4:xtic(1) t 'sliding', '' u 5:xtic(1) t 'singlelog', '' u 6:xtic(1) t 'media'
