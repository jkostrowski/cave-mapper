import csv
import os
from pathlib import Path

line = 0
home = False
home_la = 0
home_lo = 0


# that directory
files = Path(".").glob('*v1.csv')
for file in files:
    f = open(file.stem + ".v2.csv", "w")

    try: 
        with open( file, newline='') as csvfile:
            r = csv.reader(csvfile, delimiter=',', quotechar='|')
            for row in r:
                line += 1
                da = row[55]
                do = row[57]
                fq = row[59]

                row[54] = 'Q'
                row[55] = fq
                row[56] = 'DA'
                row[57] = da
                row[58] = 'DO'
                row[59] = do

                f.write( ", ".join( row ))
                f.write("\n")
    except: 
        print("{file} not complete")
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
