import pandas as pd
import numpy as np
import scipy as sp
from scipy.optimize import curve_fit, least_squares
from matplotlib import pyplot as plt
from Thermistors.DataExtractor import *
from math import *

R,Rfrac, T = ThermistorDataGetter()
print(Rfrac)

def funcT1(R, a, b, c):
    return 1 / (a + (b * np.log(R)) + (c * ((np.log(R)) **3)))

def funcT2(R, a, b, c):
    return 1 / (a + (b * np.log(R)) + (c * ((np.log(R)) **2)))

#Popt are the coefficients
#popt, pcov = curve_fit(funcT1, R, T)
popt, pcov = curve_fit(funcT2, Rfrac, T)


a, b, c = popt
#a, b, c, d = popt

print("a:", a)
print("b:", b)
print("c:", c)

#plt.plot(R, T, R, funcT1(R, a, b, c))
plt.plot(Rfrac, T, Rfrac, funcT2(Rfrac, a, b, c))
plt.title(r"$Thermistor\ T\ vs\ \frac{R}{Rref}$")
plt.legend([r"$Real\ T\ vs\ \frac{R}{Rref}$",r"$T\ =\ \frac{1}{\left( a\ +\ b*ln\frac{R}{Rref}\ +\ c*ln\frac{R}{Rref}^{2}\ \right)}$"])
plt.show()