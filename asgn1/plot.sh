#!/bin/bash

make clean && make monte_carlo
./monte_carlo -n 1000 > output.dat
awk {' print $3, $4 '} < output.dat > temp.dat		#temp holds coordinate points + x and y header
tail -n +2 temp.dat > coordinates.dat 			#coordinates holds actual points, excludes first line with x and y

#the below two functions seperate points lesser than and greater than x^2+y^2=1
#into 2 seperate files so that they can easily be represented using different colors
awk '(($1)^2 + ($2)^2) <= 1 {print $1, $2 ;} ' coordinates.dat > lessthan.dat
awk '(($1)^2 + ($2)^2) > 1 {print $1, $2 ;} ' coordinates.dat > greaterthan.dat

# heredoc for gnuplot
gnuplot <<END
	set terminal pdf
	set output "pi.pdf"
	set title "Pi estimation dot chart"
	set size square
	set tics .2
	set xlabel "x"
	set ylabel "Errors"
	set zeroaxis
	plot "lessthan.dat" with points pt 7 ps .25 lc rgb "blue" title "", \
	"greaterthan.dat" with points pt 7 ps .25 lc rgb "red" title "", \
	sqrt(1-(x**2)) with lines lc rgb "black" title ""

END

./monte_carlo -r 2 > output.dat
awk {' print $2'} < output.dat > temp.dat
tail -n +2 temp.dat > pi estimation.dat 
