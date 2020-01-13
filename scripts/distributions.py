from scipy.stats import powerlaw
import matplotlib.pyplot as plt
import numpy as np

a = .89
rvs = np.random.power(a, 10000)
for n in rvs:
    print("%0.8f"%n, end=" ")
print()

xx = np.linspace(0,1,100)
powpdf = powerlaw.pdf(xx,a)
plt.figure()
plt.hist(rvs, bins=50, density=True)
plt.plot(xx,powpdf,'r-')
plt.title('np.random.power(%d)'%a)
plt.show()
