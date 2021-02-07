import numpy as np

from readArray import read

arrQ2  = np.array([0.25, 0.35, 0.45, 0.55, 0.65])
arrW   = np.array([1.10, 1.12, 1.14, 1.16, 1.18, 1.20, 1.22, 1.24, 1.26, 1.28, 1.30, 1.32, 1.34, 1.36, 1.38, 1.40, 1.42, 1.44, 1.46, 1.48, 1.50, 1.52, 1.54, 1.56, 1.58, 1.60])
arrTh  = np.array([0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180])
arrPh  = np.array([0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360])


VAL = read("CLAS_values.txt")
ERR = read("CLAS_errors.txt")

import sys
i = int(sys.argv[1])
j = int(sys.argv[2])
k = int(sys.argv[3])

print("Data for Q2 in [%f ... %f]  W in [%f ... %f] and theta in [%f ... %f]"%(arrQ2[i], arrQ2[i+1], arrW[j], arrW[j+1], arrTh[k], arrTh[k+1]))
for l in range(12):
    print("%f ... %f = %f +- %f"%(arrPh[l], arrPh[l+1], VAL[i][j][k][l], ERR[i][j][k][l]))
