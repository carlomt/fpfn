reset                                                                                                                                                                                                     
set multiplot                                                                                                                                                                                             
                                                                                                                                                                                                          
 set size 0.35,0.35                                                                                                                                                                                       
 set origin 0.62,0.53                                                                                                                                                                                     
 set ytics 20                                                                                                                                                                                             
 set xlabel 'Time  [sec.]'                                                                                                                                                                                
 set ylabel 'Counts'                                                                                                                                                                                      
 plot 'output.txt' u 1:4 t 'thr' w l                                                                                                                                                                      
                                                                                                                                                                                                          
 reset                                                                                                                                                                                                    
 set size 1,1                                                                                                                                                                                             
 set origin 0.,0.                                                                                                                                                                                         
 set yrange [-0.01:0.31]                                                                                                                                                                                  
 set xlabel 'Time  [sec.]'                                                                                                                                                                                
 set ylabel 'Rate  [Hz]'                                                                                                                                                                                  
 plot 'output.txt' u 1:2 t 'False Positive', 'output.txt' u 1:3 t 'False Negative'                                                                                                                        
                                                                                                                                                                                                          
unset multiplot 
