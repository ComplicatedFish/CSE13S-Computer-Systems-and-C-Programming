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
	set ylabel "y"
	set zeroaxis
	plot "lessthan.dat" with points pt 7 ps .25 lc rgb "blue" title "", \
	"greaterthan.dat" with points pt 7 ps .25 lc rgb "red" title "", \
	sqrt(1-(x**2)) with lines lc rgb "black" title ""

END
for i in {2..6}
do
	./monte_carlo -r $i -n 4096 > output.dat	#runs 4096 times to fit within log scale
	awk {' print $2'} < output.dat > temp.dat	#prints second column with PI estimation
	tail -n +2 temp.dat > estimation.dat 		# removes header
	awk '{ print $1 - 3.14159 ;}' estimation.dat > error$i.dat	#determines difference between estimation and PI
done

#heredoc for gnuplot
gnuplot <<END
	set terminal pdf
	set output "error.pdf"
	set title "Monte Carlo Error Estimation"
	set xlabel "x"
	set ylabel "Errors"
	set yrange [-1:1]
	set logscale x 4
	set ytic .5
	set zeroaxis
	plot "error2.dat" with lines title "", \
	"error3.dat" with lines title "", \
	"error4.dat" with lines title "", \
	"error5.dat" with lines title "", \
	"error6.dat" with lines title ""


END

> finalestimate.dat	#file is created here to clear it
for n in {1..100}
do
	./monte_carlo -r $RANDOM -n 999 > output.dat
	awk {' print $2'} < output.dat > temp.dat
	tail -n 1 temp.dat >> finalestimate.dat
	sort finalestimate.dat > sorted.dat
done
> box.dat
awk ' ($1 < 3.0) {print $1}' sorted.dat | wc -l | awk '{print 3.0, $1}' >> box.dat
awk ' ($1 > 3.0 && $1 < 3.05) {print $1}' sorted.dat | wc -l | awk '{print 3.05, $1}' >> box.dat
awk ' ($1 > 3.05 && $1 < 3.1) {print $1}' sorted.dat | wc -l | awk '{print 3.1, $1}' >> box.dat
awk ' ($1 > 3.1 && $1 < 3.15) {print $1}' sorted.dat | wc -l | awk '{print 3.15, $1}' >> box.dat
awk ' ($1 > 3.15 && $1 < 3.2) {print $1}' sorted.dat | wc -l | awk '{print 3.2, $1}' >> box.dat
awk ' ($1 > 3.2) {print $1}' sorted.dat | wc -l | awk '{print 3.25, $1}' >> box.dat




gnuplot <<END
	set terminal pdf
	set output "frequency.pdf"
	set title "Monte Carlo frequency"
	set xlabel "x"
	set ylabel "Frequency after 999 iterations"
	set zeroaxis
	set xtics ("<3" 3, "3 - 3.05" 3.05, "3.05 - 3.1" 3.1, "3.1 - 3.15" 3.15, "3.15 - 3.2" 3.2, ">3.2" 3.25)
	set boxwidth -.1 relative
	plot "box.dat" with boxes title ""
END

