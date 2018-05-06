import sys
import matplotlib.pyplot as plt
import numpy as np
import matplotlib

x = []
y = []

with open(sys.argv[1]) as f:
    for line in f:
        data = line.split()
        y.append(int(data[0]))
        x.append(int(data[1]))

plt.scatter(np.array(x), np.array(y), c="g", alpha=0.5)
plt.xlabel("Number of Calls")
plt.ylabel("Number of Unique Paths")
plt.legend(loc=2)
plt.show()
