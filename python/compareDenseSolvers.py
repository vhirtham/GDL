import os
import sys
import tempfile
import subprocess
import json
import numpy as np
import matplotlib.pyplot as plt


def GetScriptPath():
    return os.path.dirname(os.path.realpath(sys.argv[0]))


scriptDir = GetScriptPath()
bmResultDir = scriptDir


def RunBenchmarks():
    with tempfile.TemporaryDirectory(dir=scriptDir) as buildDir:
        print('The created temporary directory is %s' % buildDir)

        cmds = "cd " + buildDir + ";"
        cmds += "cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_BENCHMARKS=true -DSOLVER_BENCHMARK_DEFINITIONS=\"-DOVERRIDE_SETUP\" ../..;"

        cmds += "echo Benchmark LU solver;"
        cmds += "make -j8 Benchmark_lu;"
        cmds += "./benchmark/math/solver/Benchmark_lu --benchmark_out_format=json --benchmark_out=" + \
                scriptDir + "/lu.json"
        subprocess.run(cmds, shell=True)


def ExtractBenchmarkSetupFromName(name):
    idx0 = name.index('/')
    idx1 = name.index('_')

    solverTypeString = name[0:idx0]
    assert (solverTypeString == 'SIMD' or solverTypeString == 'Serial')
    if solverTypeString == 'SIMD':
        simd = True
    else:
        simd = False

    pivotingString = name[idx0 + 1:idx1]

    sizeString = name[idx1 + 1: len(name)]
    idx2 = sizeString.index('x')
    size = int(sizeString[0: idx2])

    return [simd, pivotingString, size]


# RunBenchmarks()

timeScalingFactor = 1E-6

luData = lambda: 0
luData.simd = lambda: 0
luData.simd.noPivot = lambda: 0
luData.simd.noPivot.size = list()
luData.simd.noPivot.time = list()
luData.simd.noPivot.cpu = list()
luData.simd.partialPivot = lambda: 0
luData.simd.partialPivot.size = list()
luData.simd.partialPivot.time = list()
luData.simd.partialPivot.cpu = list()
luData.serial = lambda: 0
luData.serial.noPivot = lambda: 0
luData.serial.noPivot.size = list()
luData.serial.noPivot.time = list()
luData.serial.noPivot.cpu = list()
luData.serial.partialPivot = lambda: 0
luData.serial.partialPivot.size = list()
luData.serial.partialPivot.time = list()
luData.serial.partialPivot.cpu = list()

with open('lu.json') as json_file:
    fileData = json.load(json_file)
    for bm in fileData['benchmarks']:
        [simd, pivotingString, size] = ExtractBenchmarkSetupFromName(bm['name'])
        if simd:
            if pivotingString == 'NoPivot':
                luData.simd.noPivot.size.append(size)
                luData.simd.noPivot.time.append(float(bm['real_time']) * timeScalingFactor)
                luData.simd.noPivot.cpu.append(float(bm['cpu_time']) * timeScalingFactor)
            elif pivotingString == 'PartialPivot':
                luData.simd.partialPivot.size.append(size)
                luData.simd.partialPivot.time.append(float(bm['real_time']) * timeScalingFactor)
                luData.simd.partialPivot.cpu.append(float(bm['cpu_time']) * timeScalingFactor)
            else:
                assert (False)
        else:
            if pivotingString == 'NoPivot':
                luData.serial.noPivot.size.append(size)
                luData.serial.noPivot.time.append(float(bm['real_time']) * timeScalingFactor)
                luData.serial.noPivot.cpu.append(float(bm['cpu_time']) * timeScalingFactor)
            elif pivotingString == 'PartialPivot':
                luData.serial.partialPivot.size.append(size)
                luData.serial.partialPivot.time.append(float(bm['real_time']) * timeScalingFactor)
                luData.serial.partialPivot.cpu.append(float(bm['cpu_time']) * timeScalingFactor)
            else:
                assert (False)

print(luData.simd.noPivot.time)

plt.plot(luData.simd.noPivot.size, luData.simd.noPivot.time)
plt.plot(luData.simd.partialPivot.size, luData.simd.partialPivot.time)
# plt.plot(luData.serial.noPivot.size, luData.serial.noPivot.time)
# plt.plot(luData.serial.partialPivot.size, luData.serial.partialPivot.time)
plt.show()
