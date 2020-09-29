import os
import pandas as pd
import numpy as np
from math import radians,cos

os.chdir("C:\\Users\\jarre\\PycharmProjects\\ME490\\Thermistors")


def ThermistorDataGetter(header = 2):
    a = []
    filename = "ntcle305.xlsx"

    df = pd.read_excel(filename, header=header)
    R = df["RESISTANCE"].to_numpy()
    Rfrac =  df["RT/R25"].to_numpy()
    T = df["TEMPERATURE (°C)"].to_numpy()
    print("R:", R.size)
    print("T:", T.size)

    return R,Rfrac, T
