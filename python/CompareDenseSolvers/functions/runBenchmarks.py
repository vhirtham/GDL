import sys
import os
import tempfile
import subprocess


def GetScriptPath():
    return os.path.dirname(os.path.realpath(sys.argv[0]))

def RunBenchmarks():
    scriptDir = GetScriptPath()
    bmResultDir = scriptDir
    with tempfile.TemporaryDirectory(dir=scriptDir) as buildDir:
        print('The created temporary directory is %s' % buildDir)

        # Run cmake
        cmds = "cd " + buildDir + ";"
        cmds += "cmake " + \
                "-DCMAKE_BUILD_TYPE=Release " + \
                "-DENABLE_BENCHMARKS=true " + \
                "-DSOLVER_BENCHMARK_DEFINITIONS=\"-DOVERRIDE_SETUP\" " + \
                "../../..;"

        # Build and run Gauss Benchmark
        cmds += "echo Benchmark Gauss solver;"
        cmds += "make -j8 Benchmark_gauss;"
        cmds += "./benchmark/math/solver/Benchmark_gauss --benchmark_out_format=json --benchmark_out=" + \
                bmResultDir + "/Gauss.json;"

        # Build and run LU Benchmark
        cmds += "echo Benchmark LU solver;"
        cmds += "make -j8 Benchmark_lu;"
        cmds += "./benchmark/math/solver/Benchmark_lu --benchmark_out_format=json --benchmark_out=" + \
                bmResultDir + "/LU.json;"

        # Build and run QR Benchmark
        cmds += "echo Benchmark QR solver;"
        cmds += "make -j8 Benchmark_qr;"
        cmds += "./benchmark/math/solver/Benchmark_qr --benchmark_out_format=json --benchmark_out=" + \
                bmResultDir + "/QR.json;"

        # Execute command string
        subprocess.run(cmds, shell=True)