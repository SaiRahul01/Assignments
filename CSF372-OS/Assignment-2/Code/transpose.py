import numpy as np
matrix = []

with open("in2.txt") as f:
	for line in f:
		val = [i.strip() for i in line.split(' ')]
		matrix.append((val))

array = np.array(matrix,dtype = np.int64)
array_transpose = array.T
# print(array_transpose)

with open("file.txt","w") as f:
    f.write("\n".join(" ".join(map(str, x)) for x in array_transpose))