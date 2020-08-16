import time

from gdl.benchmarking import get_datetime_string
from gdl.benchmarks.base.transpose import (benchmark_all_registers,
                                           convert_json_data_to_pickle)

result_dir_extension = f"everything_{get_datetime_string()}"
# result_dir_extension = f"everything_20200813_215212"

print("Start benchmarks")
t_start = time.time()
benchmark_all_registers(True, result_dir_extension, False)
elapsed_time = "{:.2f}".format(time.time() - t_start)
print(f"Finished all benchmarks. Elapsed time: {elapsed_time} seconds")

print("Converting data")
convert_json_data_to_pickle(result_dir_extension)
print("Finished")
