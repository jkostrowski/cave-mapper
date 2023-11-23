import csv
import os
from pathlib import Path


    # //  0| 2023-11-16,  1|  17:59:58,  2| VB,  3|       0833,  4|  CS,  5|         0,  6|  CG,  7|         0,  8| CA,  9|         1, 
    # // 10|         CM, 11|         1, 12| EU, 13|        163, 14|  12, 15|      -180, 16|  GY, 17|        -0, 18| -0, 19|        -0, 
    # // 20|         LA, 21|        -0, 22| -0, 23|         -0, 24|  MA, 25|        -6, 26| -13, 27|        33, 28| AC, 29|         2,   
    # // 30|         -0, 31|       -10, 32| GR, 33|          2, 34|   0, 35|       -10, 36|   T, 37|        20, 38|  F, 39|         1, 
    # // 40|         LA, 41| 331135360, 42| LO, 43| -972332288, 44|  HD, 45| 0000.8200, 46|  VD, 47| 0000.0000, 48| PD, 49| 0000.0000,
    # missing 
    # // 50|          V, 51|  00.02000, 52|  S, 53|         10, 54|   Q, 55|         2, 56|  DA, 57|      -384, 58| DO, 59|         0


# that directory
for file in Path(".").glob('*v2.csv'):
    print( file )

    f = open(file.stem + ".v2.fixed.csv", "w")

    try: 
        with open( file, newline='') as csvfile:
            r = csv.reader(csvfile, delimiter=',', quotechar='|')

            line = 0
            home = False
            home_la = 0
            home_lo = 0

            for row in r:
                line += 1
                time = row[1]
                la   = int(row[41])
                lo   = int(row[43])

                print ( f"|{time}|" )

                if (not home and time >= "19:00:00"):
                    home = True
                    home_la = la 
                    home_lo = lo 


                da = la - home_la if home else 0
                do = lo - home_lo if home else 0

                row.append( "V"  )
                row.append( "0"  )
                row.append( "S"  )
                row.append( "5"  )
                row.append( "Q"  )
                row.append( "1"  )
                row.append( "DA" )
                row.append( str(da) )
                row.append( "DO" )
                row.append( str(do) )


                f.write( ", ".join( row ))
                f.write("\n")
    #except: 
    #    print(f"{file} not complete")
    finally:
        f.close


# 2023-11-16,17:32:26,VB,0830,
# CS,0,CG,0,CA,0,CM,1,
# EU, 187,  35, 169,
# GY,  -0,   0,  -0,
# LA,  -1,   0,  -1,
# MA,  -7,  13,  43,
# AC,   5,  -1,  -9,
# GR,   6,  -2,  -8,
# T,23,F,0,
# LA,         0,
# LO,         0,
# HD,0000.0000,VD,0000.0000,PD,0000.0000,V,00.00000,
# S,00,DA,         0,DO,         0,Q,0
