import numpy as np
import matplotlib.pyplot as plt

arr = np.array([0.130283,0.126033,0.124185,0.119668,0.128697,0.114772,0.117020,0.117493,0.114961,0.099758])

xr = np.arange(10,110,10)
plt.plot(xr,arr)
# plt.show()
plt.xlabel('Work load Size')
plt.ylabel('Waiting Time P2(seconds)')
plt.savefig('./plots-exp/wat2q1.png', bbox_inches='tight')