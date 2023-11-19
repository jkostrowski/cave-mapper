import csv
from pathlib import Path


line = 0
home = False
home_la = 0
home_lo = 0

f = open("loop.csv", "w")

# that directory
files = Path(".").glob('cave*.csv')
for file in files:
    print(file)

    with open( file, newline='') as csvfile:
        r = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in r:
            line += 1
            la  = int( row[41] )
            lo  = int( row[43] )
            hd  = row[45]
            vd  = row[47]
            vel = row[51]
            sat = row[53]
            fq  = row[59]
            # print(f' {la} | {lo} | {hd} | {vd} | {vel} | {sat} | {fq}')
            # if not home and fq == "2":
            if not home and int(sat) > 7:
                print("set home")
                home = True
                home_la = la
                home_lo = lo
            
            da = la - home_la
            do = lo - home_lo

            if home:
                row[55] = str(da) 
                row[57] = str(do)
            else:
                row[55] = "0"
                row[57] = "0"

            if (abs(da)<1_000_000 and abs(do)<1_000_000):
                f.write( ", ".join( row ))
                f.write("\n")

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
