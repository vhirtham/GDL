import time

from gdl.benchmarks.base.transpose import benchmark_transpose

register = "__m128"
rows = 2
cols = 2
offset_in = 0
offset_out = 0
result_dir_extension = "single"
verbose = True

benchmark_transpose(
    register, rows, cols, offset_in, offset_out, result_dir_extension, True
)
