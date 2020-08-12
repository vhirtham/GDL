import os
import subprocess
import sys
import tempfile


def get_script_path():
    return os.path.dirname(os.path.realpath(sys.argv[0]))


def _create_definition_string(additional_definitions_dict):
    definitions = ""
    for key, value in additional_definitions_dict.items():
        if value is None:
            value = ""
        elif isinstance(value, str):
            value = f'="{value}"'
        else:
            value = f"={str(value)}"
        definitions += f"-D{key.upper()}{value};"
    return definitions


def run_benchmark(
    benchmark_name,
    application_directory="",
    result_file_name=None,
    result_directory="",
    additional_definitions_dict={},
):
    if result_file_name is None:
        result_file_name = benchmark_name

    if not os.path.exists(result_directory):
        os.mkdir(result_directory)
        print(f"Created directory: {result_directory}")

    definition_string = _create_definition_string(additional_definitions_dict)
    dir = get_script_path()
    with tempfile.TemporaryDirectory(dir=dir) as build_dir:
        # change to build directory
        cmds = f"cd {build_dir};"

        # run cmake
        cmds += (
            "cmake "
            + "-DCMAKE_BUILD_TYPE=Release "
            + "-DENABLE_BENCHMARKS=true "
            + f'-DGDL_COMPILE_DEFINITIONS="{definition_string}" '
            + "../../../..;"
        )

        cmds += f"make -j8 Benchmark_{benchmark_name};"

        cmds += (
            f"./benchmark/{application_directory}/Benchmark_{benchmark_name} "
            f"--benchmark_out_format=json --benchmark_out={result_directory}/"
            f"{result_file_name}.json;"
        )

        # Execute command string
        subprocess.run(cmds, shell=True)
