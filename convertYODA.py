import yoda
import yaml
import numpy as np

# binning info
arrQ2  = np.array([0.25, 0.35, 0.45, 0.55, 0.65])
arrW   = np.array([1.10, 1.12, 1.14, 1.16, 1.18, 1.20, 1.22, 1.24, 1.26, 1.28, 1.30, 1.32, 1.34, 1.36, 1.38, 1.40, 1.42, 1.44, 1.46, 1.48, 1.50, 1.52, 1.54, 1.56, 1.58, 1.60])
arrTh  = np.array([0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180])
arrPh  = np.array([0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360])

from numba import jit
@jit
def findBin(arr, key):
    if (key<arr[0] or key>arr[-1]):
        return -1

    l = 0;
    for i in range(len(arr)):
        if (arr[i]>key):
            break
        l = i

    return l




def processYAML(fname):
    with open(fname) as f:
        Y = yaml.load(f, Loader=yaml.FullLoader)
        meta = Y["dependent_variables"][0]["qualifiers"]

    th=None
    w=None
    for m in meta:
        if   m['name'] == "THETA":
            th = float(m['value'])
        elif m['name'] == "W":
            w  = float(m['value'])
        else:
            pass
    if th is None:
        print(fname)
    return w,th


def serialise(arr, fname):
    with open(fname, "w") as f:
        f.write("#4 25 12 12\n")
        for i in range(4):
            for j in range(25):
                for k in range(12):
                    for l in range(12):
                        f.write("%f\n"%arr[i][j][k][l])


VAL = np.zeros([4,25,12,12])
ERR = np.zeros([4,25,12,12])


import sys

A = yoda.readYODA(sys.argv[1])

for key, v in A.items():
    tid = key.split("/")[-1].split("-x")[0]
    nid = int(tid.split("d")[-1])
    if nid <85: continue
    q2id = key.split("-y")[-1]
    i = int( key.split("-y")[-1]) - 1

    w, th = processYAML("HEPData-ins707883-v1-yaml/Table%i.yaml"%nid)
    j = findBin(arrW, w)
    k = findBin(arrTh, th)

    for p in v.points():
        phi = p.x()
        val = p.y()
        err = p.yErrAvg()
        l = findBin(arrPh, phi)
        VAL[i][j][k][l] = val
        ERR[i][j][k][l] = err


serialise(VAL, "CLAS_values.txt")
serialise(ERR, "CLAS_errors.txt")
