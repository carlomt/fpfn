reset 
 set ylabel "Amin[MBq/kg]"
 set xlabel "TNR" 
set yrange [0:30]
 

 plot  'output_Br83_8' t "Br83", 'output_P32_8' t "P32", 'output_Re188_8' t "Re188", 'output_Si31_8' t "Si31", 'output_Sm153_8' t "Sm153", 'output_Y90_8' t "Y90", 'output_Zr97_8' t "Zr97"

# plot  'output_Br83_4' t "Br83", 'output_P32_4' t "P32", 'output_Re188_4' t "Re188", 'output_Si31_4' t "Si31", 'output_Sm153_4' t "Sm153", 'output_Y90_4' t "Y90", 'output_Zr97_4' t "Zr97"

#plot  'output_Br83_1' t "Br83", 'output_P32_1' t "P32", 'output_Re188_1' t "Re188", 'output_Si31_1' t "Si31", 'output_Sm153_1' t "Sm153", 'output_Y90_1' t "Y90", 'output_Zr97_1' t "Zr97"
