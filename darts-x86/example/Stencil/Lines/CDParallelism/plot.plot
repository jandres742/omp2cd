set term png enhanced size 1024,768
set output "stencilPerformance.png"

set title "Stencil (on Intel Core 2 Quad Q6600 and 64 CD/TP)"
set xlabel "Problem Size"
set ylabel "Time (Rtdsc Cycles)"
set grid xtics ytics

plot  "openMP.txt" using 1:2 with lines title "OpenMP", \
      "sequential.txt" using 1:2 with lines title "Sequential", \
      "darts.txt" using 1:2 with lines title "DARTS"
