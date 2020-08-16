from copy import deepcopy

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm

from gdl.benchmarks.base.transpose import get_max_matrix_offsets, load_register_data

register = "__m256"
rows = 2
cols = 2
benchmark = "overwrite"
timing = "real time"


dax = load_register_data(register, "everything_20200813_215212")

max_offset_in, max_offset_out = get_max_matrix_offsets(register, rows, cols)
data = np.empty((max_offset_in + 1, max_offset_out + 1))
data[:] = np.nan
X = deepcopy(data)
Y = deepcopy(data)


for i in range(max_offset_in + 1):
    for j in range(max_offset_out + 1):
        X[i, j] = i
        Y[i, j] = j
        data[i, j] = dax.loc[rows, cols, i, j, benchmark, timing].data


fig_0 = plt.figure()
ax = fig_0.add_subplot(111, projection="3d")
ax.plot_surface(X, Y, data, cmap=cm.coolwarm)
ax.plot_wireframe(X, Y, data, color="black")
ax.set_xlim([0, max_offset_in])
ax.set_ylim([0, max_offset_out])
ax.set_zlim([0, None])
ax.set_xlabel("offset in")
ax.set_ylabel("offset out")
ax.set_zlabel(timing)

fig_1 = plt.figure()
for i in range(max_offset_in + 1):
    plt.plot(Y[i], data[i], label=f"offset in = {i}")
plt.legend()

plt.show()
