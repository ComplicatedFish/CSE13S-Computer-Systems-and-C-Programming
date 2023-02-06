#!/bin/bash

make clean && make all

> shell.dat
> batcher.dat
> heap.dat
> quick.dat
> x.dat
> y.dat
> temp.dat

for ((i=0; i<=10000; i+=50))
do
    ./sorting -s -n $i -p 0 > temp.dat
    awk {' print $5 '} < temp.dat  >> x.dat
    echo "$i" >> y.dat
done
paste y.dat x.dat > shell.dat
> x.dat
> y.dat

for ((i=0; i<=10000; i+=50))
do
    ./sorting -b -n $i -p 0 > temp.dat
    awk {' print $5 '} < temp.dat  >> x.dat
    echo "$i" >> y.dat
done
paste y.dat x.dat > batcher.dat
> x.dat
> y.dat

for ((i=0; i<=10000; i+=50))
do
    ./sorting -q -n $i -p 0 > temp.dat
    awk {' print $5 '} < temp.dat  >> x.dat
    echo "$i" >> y.dat
done
paste y.dat x.dat > quick.dat
> x.dat
> y.dat

for ((i=0; i<=10000; i+=50))
do
    ./sorting -h -n $i -p 0 > temp.dat
    awk {' print $5 '} < temp.dat  >> x.dat
    echo "$i" >> y.dat
done
paste y.dat x.dat > heap.dat

gnuplot <<END
    set terminal pdf
    set output "Moves.pdf"
    set title "Number of moves vs size using default seed"
    set xlabel "Array size (n)"
    set ylabel "Moves"
    set zeroaxis
    plot "shell.dat" with lines title "shell", \
    "heap.dat" with lines title "heap", \
    "batcher.dat" with lines title "batcher", \
    "quick.dat" with lines title "quick"


END

gnuplot <<END
    set terminal pdf
    set output "ZoomMoves.pdf"
    set title "Number of moves vs size using default seed"
    set xlabel "Array size(n)"
    set ylabel "Moves"
    set zeroaxis
    set xrange [0:100]
    set yrange [0:2000]
    plot "shell.dat" with lines title "shell", \
    "heap.dat" with lines title "heap", \
    "batcher.dat" with lines title "batcher", \
    "quick.dat" with lines title "quick"


END

> x.dat
> y.dat

for ((i=0; i<=10000; i+=50))
do
    ./sorting -s -n $i -p 0 > temp.dat
    awk {' print $7 '} < temp.dat  >> x.dat
    echo "$i" >> y.dat
done
paste y.dat x.dat > shell.dat
> x.dat
> y.dat

for ((i=0; i<=10000; i+=50))
do
    ./sorting -b -n $i -p 0 > temp.dat
    awk {' print $7 '} < temp.dat  >> x.dat
    echo "$i" >> y.dat
done
paste y.dat x.dat > batcher.dat
> x.dat
> y.dat

for ((i=0; i<=10000; i+=50))
do
    ./sorting -q -n $i -p 0 > temp.dat
    awk {' print $7 '} < temp.dat  >> x.dat
    echo "$i" >> y.dat
done
paste y.dat x.dat > quick.dat
> x.dat
> y.dat

for ((i=0; i<=10000; i+=50))
do
    ./sorting -h -n $i -p 0 > temp.dat
    awk {' print $7 '} < temp.dat  >> x.dat
    echo "$i" >> y.dat
done
paste y.dat x.dat > heap.dat

gnuplot <<END
    set terminal pdf
    set output "compares.pdf"
    set title "Number of compares vs size using default seed"
    set xlabel "Array size (n)"
    set ylabel "Comparisons"
    set zeroaxis
    plot "shell.dat" with lines title "shell", \
    "heap.dat" with lines title "heap", \
    "batcher.dat" with lines title "batcher", \
    "quick.dat" with lines title "quick"

END

gnuplot <<END
    set terminal pdf
    set output "ZoomCompares.pdf"
    set title "Number of compares vs size using default seed"
    set xlabel "Array size (n)"
    set ylabel "Comparisons"
    set zeroaxis
    set xrange [0:100]
    set yrange [0:2000]
    plot "shell.dat" with lines title "shell", \
    "heap.dat" with lines title "heap", \
    "batcher.dat" with lines title "batcher", \
    "quick.dat" with lines title "quick"

END

