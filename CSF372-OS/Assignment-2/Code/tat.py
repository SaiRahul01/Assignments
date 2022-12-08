import numpy as np
import matplotlib.pyplot as plt

arr = np.array([0.002199,0.002517,0.002251,0.001878,0.002061,0.001817,0.002461,0.001984,0.002086,0.001906])

xr = np.arange(10,110,10)
plt.plot(xr,arr)
plt.xlabel('Work load Size')
plt.ylabel('Turn around Time of P1(seconds)')
plt.show()