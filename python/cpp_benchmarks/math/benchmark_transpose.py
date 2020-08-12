import gdl.benchmarking as bm


def is_register_valid(register):
    return register in ["__m128", "__m128d", "__m256", "__m256d"]


def benchmark_transpose(register, rows, cols, offset_in, offset_out):
    if not is_register_valid(register):
        raise ValueError(f'"{register}" is an invalid register type')
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
    )


benchmark_transpose("__m256", 4, 4, 0, 0)
