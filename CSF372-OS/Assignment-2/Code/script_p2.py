import csv
import numpy as np
import matplotlib.pyplot as plt 
lis = ''
f1 = open("graph_p2.txt","r")
 
lis = f1.read().split()
lis = list(map(float,lis))
 
# print(lis)
sz = len(lis)
plt.plot(np.arange(1,sz+1,1), lis)
plt.xlim([1,len(lis)])
plt.xlabel('Number of Threads')
plt.ylabel('Time (Nano Seconds)')
# plt.show()
plt.savefig('plot_p2.png',bbox_inches='tight')