"""Functions to run c++ benchmarks using python"""

import os
import subprocess
import tempfile
from typing import Dict, Union

from gdl.utility import get_script_path


def _create_definition_string(additional_definitions_dict: Dict) -> str:
    """Create a additional compile definitions string that can be passed to CMake.

    Note that this only works with the GDL CMake script.

    Parameters
    ----------
    additional_definitions_dict:
        Dictionary with additional definitions. Each key must be a string and will be
        capitalized. The value of the definition is equal to the dictionaries value.
        If the dictionaries value is 'None', only the name without a value will be
        passed as additional compile time definition.

    Returns
    -------
    str:
        String with additional definitions that can be passed to the GDL CMake script.
    """
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
    benchmark_name: str,
    source_directory: str = "",
    result_file_name: Union[str, None] = None,
    result_directory: str = "",
    additional_definitions_dict={},
    verbose=True,
):
    """Compile and run a GDL benchmark.

    Parameters
    ----------
    benchmark_name:
        Name of the benchmark without the "Benchmark_" prefix.
    source_directory:
        Subdirectory of the benchmarks source files.
    result_file_name:
        Name of the result file. If 'None' is passed, it is equal to the benchmark name.
    result_directory:
        Name of the result directory
    additional_definitions_dict:
        Dictionary with additional definitions. Each key must be a string and will be
        capitalized. The value of the definition is equal to the dictionaries value.
        If the dictionaries value is 'None', only the name without a value will be
        passed as additional compile time definition.
    verbose:
        If 'True', the output of the subprocess shell is printed.

    """
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
            f"./benchmark/{source_directory}/Benchmark_{benchmark_name} "
            f"--benchmark_out_format=json --benchmark_out={result_directory}/"
            f"{result_file_name}.json;"
        )

        # Execute command string
        if verbose:
            subprocess.run(cmds, shell=True)
        else:
            print(f"running {result_file_name}")
            FNULL = open(os.devnull, "w")
            subprocess.run(cmds, stdout=FNULL, stderr=FNULL, shell=True)
