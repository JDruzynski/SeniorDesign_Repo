import pandas as pd
import numpy as np
import scipy as sp
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt
from Radiometer.DataExtractor import *

def funcL(X, a, b, c, n):
    V, T_opt, T_d, cosTh = X
    return (a*V - b*(T_opt**n) + c*(T_d**n)) / cosTh

L, V, T_opt, T_d, cosTh = radDataGetter("",delimeter=",")

inputs = (V, T_opt, T_d, cosTh)

# initial guesses for a,b,c,n:
p0 = 8., 2., 7., 4.

#Popt are the coefficients
popt, pcov = curve_fit(funcL, inputs, L, p0)

a, b, c, n = popt