import json
import os
import pickle
from typing import List, Tuple

import numpy as np
from xarray import DataArray

import gdl.benchmarking as bm


def get_result_directory_name(result_dir_extension: str) -> str:
    """Get the default result directory name with added extension

    Parameters
    ----------
    result_dir_extension
        String that should be added as suffix to the default directory name

    Returns
    -------
    str:
        Default result directory name with added extension

    """
    if result_dir_extension == "":
        return "results_benchmark_transpose"
    return f"results_benchmark_transpose_{result_dir_extension}"


def filter_matching_strings(list_of_strings: List, matches: List) -> List:
    """Return all strings from a list that contain all the passed substrings.

    Parameters
    ----------
    list_of_strings:
        List of strings
    matches:
        List of substrings

    Returns
    -------
    List:
        List of strings that contain all passed substrings

    """
    return [
        string
        for string in list_of_strings
        if all(match in string for match in matches)
    ]


def supported_registers() -> List:
    """Get a list of supported vector registers.

    Returns
    -------
    List:
        List with supported vector registers

    """
    return ["__m128d", "__m128", "__m256d", "__m256"]


def is_supported_register(register: str) -> bool:
    """Return 'True' if the passed string has the name of a supported vector register.

    Parameters
    ----------
    register:
        Name of a vector register type

    Returns
    -------
    bool:
        'True' if the passed string has the name of a supported vector register.
        'False' otherwise

    """
    return register in supported_registers()


def num_register_elements(register: str) -> int:
    """Get the number of vector register elements.

    Parameters
    ----------
    register:
        Name of a vector register type

    Returns
    -------
    int:
        Number of vector register elements

    """
    if not is_supported_register(register):
        raise ValueError(f'"{register}" is an invalid register type')

    register_sizes = {"__m128": 4, "__m128d": 2, "__m256": 8, "__m256d": 4}
    return register_sizes[register]


def get_max_matrix_offsets(register, rows, cols) -> Tuple:
    """Get the maximum input and output offset for a given matrix size.

    Parameters
    ----------
    register:
        Name of a vector register type
    rows:
        Number of matrix rows
    cols:
        Number of matrix columns

    Returns
    -------
    Tuple:
        Maximum input and output offset.

    """
    register_size = num_register_elements(register)
    max_offset_in = register_size - rows
    max_offset_out = register_size - cols

    return max_offset_in, max_offset_out


# benchmarks ---------------------------------------------------------------------------


def benchmark_transpose(
    register: str,
    rows: int,
    cols: int,
    offset_in: int,
    offset_out: int,
    result_dir_extension: str = "",
    verbose: bool = True,
):
    """Run a single benchmark for the GDL Transpose function.

    Parameters
    ----------
    register:
        Name of a vector register type
    rows:
        Number of matrix rows
    cols:
        Number of matrix columns
    offset_in:
        Input offset of the matrix
    offset_out:
        Output offset of the matrix
    result_dir_extension
        String that should be added as suffix to the default directory name
    verbose:
        If 'True', the output of the subprocess shell is printed.

    """
    register_size = num_register_elements(register)
    if rows > register_size or cols > register_size:
        raise ValueError(
            f"Matrix exceeds register size. Maximum is {register_size}."
            f"Provided values - rows:{rows} - cols:{cols}"
        )

    result_file_name = (
        f"transpose_{rows}x{cols}_{register[2:]}_in_{offset_in}_out_{offset_out}"
    )
    result_directory = (
        f"{bm.get_script_path()}/{get_result_directory_name(result_dir_extension)}"
    )

    definitions = {
        "register": register,
        "rows": rows,
        "cols": cols,
        "offset_in": offset_in,
        "offset_out": offset_out,
    }

    bm.run_benchmark(
        "transpose",
        source_directory="base/sse",
        result_file_name=result_file_name,
        result_directory=result_directory,
        additional_definitions_dict=definitions,
        verbose=verbose,
    )


def benchmark_all_offset_combinations(
    register: str,
    rows: int,
    cols: int,
    result_dir_extension: str = "",
    verbose: bool = True,
):
    """Run a 'Transpose' benchmarks for all offset combinations for the given matrix.

    Parameters
    ----------
    register:
        Name of a vector register type
    rows:
        Number of matrix rows
    cols:
        Number of matrix columns
    result_dir_extension:
        String that should be added as suffix to the default directory name
    verbose:
        If 'True', the output of the subprocess shell is printed.

    """
    register_size = num_register_elements(register)
    max_offset_in = register_size - rows
    max_offset_out = register_size - cols

    for i in range(max_offset_in + 1):
        for o in range(max_offset_out + 1):
            benchmark_transpose(
                register, rows, cols, i, o, result_dir_extension, verbose,
            )


def benchmark_all_sizes(
    register: str,
    all_offsets: bool = False,
    result_dir_extension: str = "",
    verbose: bool = True,
):
    """Benchmark the 'Transpose' function for all matrix sizes.

    Parameters
    ----------
    register:
        Name of a vector register type
    all_offsets:
        If 'True', all offset combinations are benchmarked. If 'False', the offsets are
        set to 0.
    result_dir_extension:
        String that should be added as suffix to the default directory name
    verbose:
        If 'True', the output of the subprocess shell is printed.

    Returns
    -------

    """
    register_size = num_register_elements(register)
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


def benchmark_all_registers(
    all_offsets: bool = False, result_dir_extension: str = "", verbose: bool = True
):
    """Run 'Transpose' function benchmarks for all supported register types.

    Parameters
    ----------
    all_offsets:
        If 'True', all offset combinations are benchmarked. If 'False', the offsets are
        set to 0.
    result_dir_extension:
        String that should be added as suffix to the default directory name
    verbose:
        If 'True', the output of the subprocess shell is printed.



    """
    registers = ["__m128d", "__m128", "__m256d", "__m256"]
    for register in registers:
        benchmark_all_sizes(register, all_offsets, result_dir_extension, verbose)


# data processing ----------------------------------------------------------------------


def _extract_offsets_from_file_name(file_name: str) -> Tuple:
    """Extract the offsets from the result file name.

    Parameters
    ----------
    file_name:
        Name of the result file

    Returns
    -------
    Tuple:
        Matrix input and output offset

    """
    pos_in = file_name.find("_in_")
    pos_out = file_name.find("_out_")
    pos_json = file_name.find(".json")
    offset_in = int(file_name[pos_in + 4 : pos_out])
    offset_out = int(file_name[pos_out + 5 : pos_json])

    return offset_in, offset_out


def _create_matrix_data_array(
    file_names: List, directory: str, register: str, rows: int, cols: int
) -> DataArray:
    """Load register specific results for a certain matrix size into a xarray.DataArray.

    Parameters
    ----------
    file_names:
        List of result file names
    directory:
        Directory of the result files
    register:
        Name of a vector register type
    rows:
        Number of matrix rows
    cols:
        Number of matrix columns

    Returns
    -------
    DataArray:
        xarray.DataArray containing all benchmark results for the given matrix size

    """
    dax = _get_preallocated_matrix_data_array(register, rows, cols)

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


def _get_preallocated_matrix_data_array(
    register: str, rows: int, cols: int
) -> DataArray:
    """Get a preallocated xarray.DataArray for a given matrix size.

    The memory to store all possible value combinations is already allocated and filled
    with NaNs.

    Parameters
    ----------
    register:
        Name of a vector register type
    rows:
        Number of matrix rows
    cols:
        Number of matrix columns

    Returns
    -------
    DataArray
        Preallocated xarray.DataArray filled with NaNs

    """
    return _get_preallocated_register_data_array(register).loc[rows, cols]


def _get_preallocated_register_data_array(register):
    """Get a preallocated xarray.DataArray for a given register type.

    The memory to store all possible value combinations is already allocated and filled
    with NaNs.

    Parameters
    ----------
    register:
        Name of a vector register type

    Returns
    -------
    DataArray
        Preallocated xarray.DataArray filled with NaNs
    """
    size = num_register_elements(register)
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


def convert_register_data_to_data_array(
    register: str, result_dir_extension: str
) -> DataArray:
    """Load all available data for a register and store it in a xarray.DataArray.

    Parameters
    ----------
    register:
        Name of a vector register type
    result_dir_extension:
        String that should be added as suffix to the default directory name

    Returns
    -------
    DataArray:
        xarray.DataArray containing all register specific benchmark data.
    """
    dir_name = get_result_directory_name(result_dir_extension)

    file_names = filter_matching_strings(
        os.listdir(dir_name), ["transpose_", register[1:], ".json"]
    )

    size_range = np.arange(1, num_register_elements(register) + 1)
    dax = _get_preallocated_register_data_array(register)

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
    """Convert all data in the result directory to DataArrays and save them with pickle.

    Parameters
    ----------
    result_dir_extension:
        String that should be added as suffix to the default directory name

    """
    registers = supported_registers()
    dir_name = get_result_directory_name(result_dir_extension)

    for register in registers:
        dax = convert_register_data_to_data_array(register, result_dir_extension)

        file_name = f"transpose{register[1:]}.pkl"
        with open(f"{dir_name}/{file_name}", "wb") as output:
            pickle.dump(dax, output, pickle.HIGHEST_PROTOCOL)


def load_register_data(register: str, result_dir_extension: str) -> DataArray:
    """Load all data for a given register type.
    
    Parameters
    ----------
    register:
        Name of a vector register type
    result_dir_extension:
        String that should be added as suffix to the default directory name

    Returns
    -------
    DataArray
        xarray.DataArray containing the register data
    """
    dir_name = get_result_directory_name(result_dir_extension)
    file_name = f"transpose{register[1:]}.pkl"
    with open(f"{dir_name}/{file_name}", "rb") as input:
        return pickle.load(input)
