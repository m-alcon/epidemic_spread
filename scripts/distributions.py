#!/usr/bin/python3

from scipy.stats import powerlaw
import matplotlib.pyplot as plt
import numpy as np
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('alpha', help='Exponent alpha.')
parser.add_argument('max', help='Maximum possible value.')
args = parser.parse_args()

a = float(args.alpha)
b = int(args.max)
rvs = np.random.power(a, 10000)
for n in rvs:
    #n = np.random.choice([np.ceil(n),np.floor(n)])
    print("%0.5f"%n, end=" ")
print()

h = np.histogram(rvs, 50)


plt.figure()
plt.plot(h[0],h[1][:-1])

#xx = np.linspace(0,1,100)
# powpdf = powerlaw.pdf(xx,a)
# plt.figure()
# plt.hist(rvs, bins=50, density=True)
# plt.plot(xx,powpdf,'r-')
# plt.title('np.random.power(%0.2f)'%a)
plt.xscale("log")
plt.yscale("log")
plt.show()
