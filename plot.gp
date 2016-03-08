reset
set multiplot

 set size 0.35,0.35
 set origin 0.62,0.53
 set ytics 20
 set xlabel 'Time  [sec.]'
 set ylabel 'Counts'
 plot 'output.txt' u 1:4 t 'thr' w l

 reset
 sf = 100
 set size 1,1
 set origin 0.,0.
# set yrange [-0.01*sf:0.31*sf]
 set xlabel 'Time  [sec.]'
 set ylabel 'Prob.  [%]'
 plot 'output.txt' u 1:($2*sf) t 'False Positive', 'output.txt' u 1:($3*sf) t 'False Negative'
 
unset multiplot
 
