import numpy as np

import gdl.benchmarking as bm


def is_register_valid(register):
    return register in ["__m128", "__m128d", "__m256", "__m256d"]


def get_register_size(register):
    if not is_register_valid(register):
        raise ValueError(f'"{register}" is an invalid register type')

    register_sizes = {"__m128": 4, "__m128d": 2, "__m256": 8, "__m256d": 4}
    return register_sizes[register]


def benchmark_transpose(register, rows, cols, offset_in, offset_out, verbose=True):
    register_size = get_register_size(register)
    benchmark_name = "transpose"
    result_file_name = (
        f"{benchmark_name}_{rows}x{cols}_{register[2:]}_in_{offset_in}_out_{offset_out}"
    )
    result_directory = f"{bm.get_script_path()}/results_benchmark_{benchmark_name}"
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


def benchmark_all_offset_combinations(register, rows, cols, verbose=True):
    register_size = get_register_size(register)
    max_offset_in = register_size - rows
    max_offset_out = register_size - cols

    for i in range(max_offset_in + 1):
        for o in range(max_offset_out + 1):
            benchmark_transpose(register, rows, cols, i, o, verbose=verbose)


def benchmark_all_sizes(register, all_offsets=False, verbose=True):
    register_size = get_register_size(register)
    if all_offsets:
        for r in np.arange(1, register_size + 1, 1):
            for c in np.arange(1, register_size + 1, 1):
                benchmark_all_offset_combinations(register, r, c, verbose=verbose)
    else:
        for r in np.arange(1, register_size + 1, 1):
            for c in np.arange(1, register_size + 1, 1):
                benchmark_transpose(register, r, c, 0, 0, verbose=verbose)


benchmark_all_sizes("__m128d", True, verbose=False)
