import sys
import os
import tempfile
import subprocess


def get_script_path():
    return os.path.dirname(os.path.realpath(sys.argv[0]))

def run_benchmark():
    dir = get_script_path()
    with tempfile.TemporaryDirectory(dir=dir) as build_dir:
        # change to build directory
        cmds = f"cd {build_dir};"

        # run cmake
        cmds += "cmake " + \
                "-DCMAKE_BUILD_TYPE=Release " + \
                "-DENABLE_BENCHMARKS=true " + \
                '-DSOLVER_BENCHMARK_DEFINITIONS="-DOVERRIDE_SETUP" ' + \
                "../../../..;"

        # Execute command string
        subprocess.run(cmds, shell=True)

