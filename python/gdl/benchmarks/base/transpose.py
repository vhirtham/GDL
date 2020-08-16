import json
import os
import pickle

import numpy as np
from xarray import DataArray

import gdl.benchmarking as bm


def is_register_valid(register):
    return register in ["__m128", "__m128d", "__m256", "__m256d"]


def get_register_size(register):
    if not is_register_valid(register):
        raise ValueError(f'"{register}" is an invalid register type')

    register_sizes = {"__m128": 4, "__m128d": 2, "__m256": 8, "__m256d": 4}
    return register_sizes[register]


def get_max_matrix_offsets(register, rows, cols):
    register_size = get_register_size(register)
    max_offset_in = register_size - rows
    max_offset_out = register_size - cols

    return max_offset_in, max_offset_out


def benchmark_transpose(
    register, rows, cols, offset_in, offset_out, result_dir_extension="", verbose=True
):
    register_size = get_register_size(register)
    if rows > register_size or cols > register_size:
        raise ValueError(
            f"Matrix exceeds register size. Maximum is {register_size}."
            f"Provided values - rows:{rows} - cols:{cols}"
        )
    benchmark_name = "transpose"
    result_file_name = (
        f"{benchmark_name}_{rows}x{cols}_{register[2:]}_in_{offset_in}_out_{offset_out}"
    )
    result_directory = f"{bm.get_script_path()}/results_benchmark_{benchmark_name}"
    if result_dir_extension != "":
        result_directory = f"{result_directory}_{result_dir_extension}"

    definitions = {
        "register": register,
        "rows": rows,
        "cols": cols,
        "offset_in": offset_in,
        "offset_out": offset_out,
    }

    bm.run_benchmark(
        benchmark_name,
        application_directory="base/sse",
        result_file_name=result_file_name,
        result_directory=result_directory,
        additional_definitions_dict=definitions,
        verbose=verbose,
    )


def benchmark_all_offset_combinations(
    register, rows, cols, result_dir_extension="", verbose=True
):
    register_size = get_register_size(register)
    max_offset_in = register_size - rows
    max_offset_out = register_size - cols

    for i in range(max_offset_in + 1):
        for o in range(max_offset_out + 1):
            benchmark_transpose(
                register, rows, cols, i, o, result_dir_extension, verbose,
            )


def benchmark_all_sizes(
    register, all_offsets=False, result_dir_extension="", verbose=True
):
    register_size = get_register_size(register)
    if all_offsets:
        for r in np.arange(1, register_size + 1, 1):
            for c in np.arange(1, register_size + 1, 1):
                benchmark_all_offset_combinations(
                    register, r, c, result_dir_extension, verbose
                )
    else:
        for r in np.arange(1, register_size + 1, 1):
            for c in np.arange(1, register_size + 1, 1):
                benchmark_transpose(register, r, c, 0, 0, result_dir_extension, verbose)


def benchmark_all_registers(all_offsets=False, result_dir_extension="", verbose=True):
    registers = ["__m128d", "__m128", "__m256d", "__m256"]
    for register in registers:
        benchmark_all_sizes(register, all_offsets, result_dir_extension, verbose)


def _extract_offsets_from_file_name(file_name):
    pos_in = file_name.find("_in_")
    pos_out = file_name.find("_out_")
    pos_json = file_name.find(".json")
    offset_in = int(file_name[pos_in + 4 : pos_out])
    offset_out = int(file_name[pos_out + 5 : pos_json])

    return offset_in, offset_out


def _create_matrix_data_array(file_names, directory, register, rows, cols):

    dax = _get_empty_matrix_data_array(register, rows, cols)

    for file_name in file_names:
        offset_in, offset_out = _extract_offsets_from_file_name(file_name)

        with open(f"{directory}/{file_name}") as json_file:
            file_data = json.load(json_file)
            for benchmark in file_data["benchmarks"]:

                if "overwrite" in benchmark["name"]:
                    bm = "overwrite"
                elif "set_zero" in benchmark["name"]:
                    bm = "set zero"
                else:
                    bm = "keep unused"
                dax[offset_in, offset_out].loc[bm, "cpu time"] = benchmark["cpu_time"]
                dax[offset_in, offset_out].loc[bm, "real time"] = benchmark["real_time"]
    return dax


def _get_empty_register_data_array(register):
    size = get_register_size(register)
    data = np.empty((size, size, size, size, 3, 2), dtype=float)
    data[:] = np.nan

    offset_range = np.arange(0, size)
    size_range = offset_range + 1

    return DataArray(
        name=register,
        data=data,
        dims=["rows", "cols", "offset in", "offset out", "benchmark", "timing"],
        coords={
            "rows": size_range,
            "cols": size_range,
            "offset in": offset_range,
            "offset out": offset_range,
            "benchmark": ["overwrite", "set zero", "keep unused"],
            "timing": ["cpu time", "real time"],
        },
    )


def filter_matching_strings(list_of_strings, matches):
    return [
        string
        for string in list_of_strings
        if all(match in string for match in matches)
    ]


def _get_empty_matrix_data_array(register, rows, cols):
    return _get_empty_register_data_array(register).loc[rows, cols]


def get_result_directory_name(result_dir_extension):
    return f"results_benchmark_transpose_{result_dir_extension}"


def convert_register_data_to_data_array(register, result_dir_extension):
    dir_name = get_result_directory_name(result_dir_extension)

    file_names = filter_matching_strings(
        os.listdir(dir_name), ["transpose_", register[1:], ".json"]
    )

    size_range = np.arange(1, get_register_size(register) + 1)
    dax = _get_empty_register_data_array(register)

    missing_sizes = set()
    incomplete_sizes = set()

    for r in size_range:
        for c in size_range:
            f_names = filter_matching_strings(file_names, [f"_{r}x{c}_"])

            dax.loc[r, c] = _create_matrix_data_array(f_names, dir_name, register, r, c)

            max_offset_in, max_offset_out = get_max_matrix_offsets(register, r, c)
            num_offset_comb = (max_offset_out + 1) * (max_offset_in + 1)

            if len(f_names) != num_offset_comb:
                if len(f_names) == 0:
                    missing_sizes.add(f"{r}x{c}")
                else:
                    incomplete_sizes.add(f"{r}x{c}")

    dax.attrs["missing sizes"] = list(missing_sizes)
    dax.attrs["incomplete sizes"] = list(incomplete_sizes)

    return dax


def convert_json_data_to_pickle(result_dir_extension):
    registers = ["__m128d", "__m128", "__m256d", "__m256"]
    dir_name = get_result_directory_name(result_dir_extension)

    for register in registers:
        dax = convert_register_data_to_data_array(register, result_dir_extension)

        file_name = f"transpose{register[1:]}.pkl"
        with open(f"{dir_name}/{file_name}", "wb") as output:
            pickle.dump(dax, output, pickle.HIGHEST_PROTOCOL)


def load_register_data(register, result_dir_extension):
    dir_name = get_result_directory_name(result_dir_extension)
    file_name = f"transpose{register[1:]}.pkl"
    with open(f"{dir_name}/{file_name}", "rb") as input:
        return pickle.load(input)
