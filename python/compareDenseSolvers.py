import os
import sys
import tempfile
import subprocess
import json
import matplotlib.pyplot as plt

### -------------------------------------------------------------------------------------------------------------------
### Setup
### -------------------------------------------------------------------------------------------------------------------


runBenchmarks = False


### Classes -----------------------------------------------------------------------------------------------------------

class BenchmarkResults:
    def __init__(self, solverName, typeName, pivotName):
        self.solverName = solverName
        self.pivotName = pivotName
        self.typeName = typeName
        self.size = list()
        self.time = list()
        self.cpu = list()

    def SetVectorizationName(self, vectorizationName):
        self.vectorizationName = vectorizationName
        self.benchmarkName = self.solverName + " - " + vectorizationName + " - " + self.typeName + " - " + self.pivotName
        self.benchmarkNameNoType = self.solverName + " - " + vectorizationName + " - " + self.pivotName
        self.benchmarkNameShort = self.solverName + " - " + self.pivotName


class SolverBenchmarkData:

    def __init__(self, solverName, simdTypeName='SIMD'):
        self.simd = lambda: 0
        self.simd.F32 = lambda: 0
        self.simd.F64 = lambda: 0
        self.simd.F32.noPivot = BenchmarkResults(solverName, 'F32', 'no pivoting')
        self.simd.F32.partialPivot = BenchmarkResults(solverName, 'F32', 'partial pivoting')
        self.simd.F64.noPivot = BenchmarkResults(solverName, 'F64', 'no pivoting')
        self.simd.F64.partialPivot = BenchmarkResults(solverName, 'F64', 'partial pivoting')
        self.serial = lambda: 0
        self.serial.F32 = lambda: 0
        self.serial.F64 = lambda: 0
        self.serial.F32.noPivot = BenchmarkResults(solverName, 'F32', 'no pivoting')
        self.serial.F32.partialPivot = BenchmarkResults(solverName, 'F32', 'partial pivoting')
        self.serial.F64.noPivot = BenchmarkResults(solverName, 'F64', 'no pivoting')
        self.serial.F64.partialPivot = BenchmarkResults(solverName, 'F64', 'partial pivoting')
        self.serial.F32.noPivot.SetVectorizationName('Serial')
        self.serial.F32.partialPivot.SetVectorizationName('Serial')
        self.serial.F64.noPivot.SetVectorizationName('Serial')
        self.serial.F64.partialPivot.SetVectorizationName('Serial')
        self.vectorizationNameSet = False

    def SetVectorizationName(self, vectorizationName):
        if not self.vectorizationNameSet:
            self.simd.F32.noPivot.SetVectorizationName(vectorizationName)
            self.simd.F32.partialPivot.SetVectorizationName(vectorizationName)
            self.simd.F64.noPivot.SetVectorizationName(vectorizationName)
            self.simd.F64.partialPivot.SetVectorizationName(vectorizationName)
            self.vectorizationNameSet = True


### Functions ---------------------------------------------------------------------------------------------------------

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
                "../..;"

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

        # Execute command string
        subprocess.run(cmds, shell=True)


def ExtractBenchmarkSetupFromName(name):
    idx = name.index('_')
    vectorizationString = name[0:idx]
    assert (vectorizationString == 'SSE' or vectorizationString == 'AVX' or vectorizationString == 'Serial')
    if vectorizationString == 'Serial':
        simd = False
    else:
        simd = True

    remaining = name[idx + 1:len(name)]
    idx = remaining.index('/')
    typeString = remaining[0:idx]

    remaining = remaining[idx + 1:len(remaining)]
    idx = remaining.index('_')
    pivotingString = remaining[0:idx]

    remaining = remaining[idx + 1:len(remaining)]
    idx = remaining.index('x')
    size = int(remaining[0: idx])

    return [simd, typeString, vectorizationString, pivotingString, size]


def AppendBenchmarkResults(bmResults, bmData, size):
    bmResults.size.append(size)
    bmResults.time.append(float(bmData['real_time']) * timeScalingFactor)
    bmResults.cpu.append(float(bmData['cpu_time']) * timeScalingFactor)


def ReadSolverBenchmarkData(solverName):
    solverBenchmarkData = SolverBenchmarkData(solverName)
    fileName = solverName + '.json'

    with open(fileName) as json_file:
        fileData = json.load(json_file)
        for bm in fileData['benchmarks']:
            [simd, typeString, vectorizationString, pivotingString, size] = ExtractBenchmarkSetupFromName(bm['name'])
            if typeString == 'F32':
                if simd:
                    solverBenchmarkData.SetVectorizationName(vectorizationString)
                    if pivotingString == 'NoPivot':
                        AppendBenchmarkResults(solverBenchmarkData.simd.F32.noPivot, bm, size)
                    elif pivotingString == 'PartialPivot':
                        AppendBenchmarkResults(solverBenchmarkData.simd.F32.partialPivot, bm, size)
                    else:
                        assert (False)
                else:
                    if pivotingString == 'NoPivot':
                        AppendBenchmarkResults(solverBenchmarkData.serial.F32.noPivot, bm, size)
                    elif pivotingString == 'PartialPivot':
                        AppendBenchmarkResults(solverBenchmarkData.serial.F32.partialPivot, bm, size)
                    else:
                        assert (False)
            else:
                if simd:
                    solverBenchmarkData.SetVectorizationName(vectorizationString)
                    if pivotingString == 'NoPivot':
                        AppendBenchmarkResults(solverBenchmarkData.simd.F64.noPivot, bm, size)
                    elif pivotingString == 'PartialPivot':
                        AppendBenchmarkResults(solverBenchmarkData.simd.F64.partialPivot, bm, size)
                    else:
                        assert (False)
                else:
                    if pivotingString == 'NoPivot':
                        AppendBenchmarkResults(solverBenchmarkData.serial.F64.noPivot, bm, size)
                    elif pivotingString == 'PartialPivot':
                        AppendBenchmarkResults(solverBenchmarkData.serial.F64.partialPivot, bm, size)
                    else:
                        assert (False)

    return solverBenchmarkData


### -------------------------------------------------------------------------------------------------------------------
### Main
### -------------------------------------------------------------------------------------------------------------------


if runBenchmarks:
    RunBenchmarks()

timeScalingFactor = 1

gaussData = ReadSolverBenchmarkData('Gauss')
luData = ReadSolverBenchmarkData('LU')

# plt.plot(luData.simd.F32.noPivot.size, luData.simd.F32.noPivot.time, label=luData.simd.F32.noPivot.benchmarkName)
# plt.plot(gaussData.simd.F32.noPivot.size, gaussData.simd.F32.noPivot.time,
#         label=gaussData.simd.F32.noPivot.benchmarkName)
plt.plot(luData.serial.F32.noPivot.size, luData.serial.F32.noPivot.time, label=luData.serial.F32.noPivot.benchmarkName)
plt.plot(gaussData.serial.F32.noPivot.size, gaussData.serial.F32.noPivot.time,
         label=gaussData.serial.F32.noPivot.benchmarkName)
# plt.plot(luData.simd.F64.noPivot.size, luData.simd.F64.noPivot.time, label=luData.simd.F64.noPivot.benchmarkName)
# plt.plot(luData.simd.F32.partialPivot.size, luData.simd.F32.partialPivot.time, label=luData.simd.F32.partialPivot.benchmarkName)
# plt.plot(luData.serial.F32.noPivot.size, luData.serial.F32.noPivot.time)
# plt.plot(luData.serial.F32.partialPivot.size, luData.serial.F32.partialPivot.time)
plt.legend()

plt.show()
