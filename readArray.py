import numpy as np

def read(fname):
    # First line contains array dimensions
    with open(fname, "r") as f:
        line=f.readline().strip()
        l=line.strip("#")
        dims = [int(i) for i in l.split()]

        DATA = np.zeros(dims)

        # We know it is a 4D array
        for i in range(dims[0]):
            for j in range(dims[1]):
                for k in range(dims[2]):
                    for l in range(dims[3]):
                        d=f.readline().strip()
                        DATA[i][j][k][l] = float(d)
        return DATA






if __name__ ==  "__main__":
    import sys
    X= read(sys.argv[1])
