import os
import pandas as pd
import numpy as np
from math import radians,cos


def radDataGetter(directory, delimeter = '\t', header = 1,condFunc = "Mode"):
    a = []
    for filename in os.listdir(directory):
        if filename.endswith(".csv"):
            with pd.read_csv(filename,delimeter = delimeter,header=header) as df:
                if condFunc == "Mean":
                    V, T_opt, T_d = df.mean()
                elif condFunc == "Mode":
                    V, T_opt, T_d = df.mode()

            with open(filename) as f:
                firstLine = f.readline()
                theta, T_bb, L = firstLine.split(',')


            tempList = [L, V, T_opt, T_d, cos(radians(theta))]
            a = a.append(tempList)

        else:
            continue

    a = np.array(a)
    return a.T
