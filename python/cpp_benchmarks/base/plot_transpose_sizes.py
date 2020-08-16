from copy import deepcopy

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm

from gdl.benchmarks.base.transpose import (load_register_data,
                                           num_register_elements)

register = "__m256"
benchmark = "overwrite"
timing = "real time"
offset_in = 0
offset_out = 0


dax = load_register_data(register, "everything_20200813_215212")

size = num_register_elements(register)
size_range = np.arange(1, size + 1)
data = np.empty((size - offset_in, size - offset_out))
data[:] = np.nan
X = deepcopy(data)
Y = deepcopy(data)


for i in range(size - offset_in):
    for j in range(size - offset_out):
        X[i, j] = i + 1
        Y[i, j] = j + 1
        data[i, j] = dax.loc[
            i + 1, j + 1, offset_in, offset_out, benchmark, timing
        ].data


fig = plt.figure()
ax = fig.add_subplot(111, projection="3d")
ax.plot_surface(X, Y, data, cmap=cm.coolwarm)
ax.plot_wireframe(X, Y, data, color="black")
ax.set_xlim([1, size - offset_in])
ax.set_ylim([1, size - offset_out])
ax.set_zlim([0, None])
ax.set_xlabel("rows")
ax.set_ylabel("columns")
ax.set_zlabel(timing)
plt.show()
