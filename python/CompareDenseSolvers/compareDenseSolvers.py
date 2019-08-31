import sys
import copy
import matplotlib.pyplot as plt

sys.path.append('functions')

from runBenchmarks import *
from readBenchmarkResults import *

### -------------------------------------------------------------------------------------------------------------------
### Setup
### -------------------------------------------------------------------------------------------------------------------


runBenchmarks = False

compareSolvers = True
compareSolversSIMD = True
compareSolversDataType = 'F64'
compareSolversPivot = 'PartialPivot'
compareSolversReferenceSolver = 'LU'

compareSerialSimd = True
compareSerialSimdDataType = 'F64'
compareSerialSimdPivot = 'NoPivot'

luColor = (1, 0, 0)
gaussColor = (0, 0, 1)
qrColor = (0, 1, 0)


### -------------------------------------------------------------------------------------------------------------------

def PlotRelativePerformance(benchmarks, idxRef=0):
    fig = plt.figure()
    for benchmark in benchmarks:
        benchmarkName = benchmark.GetName()
        if len(benchmark.size) == 0:
            print('No data for benchmark: ' + benchmarkName)
            continue
        else:
            time = copy.deepcopy(benchmark.time)
            for i in range(len(time)):
                time[i] /= benchmarks[idxRef].time[i]

            plt.plot(benchmark.size, time, color=benchmark.color, label=benchmarkName)

    plt.title('Relative performance to:\n' + benchmarks[idxRef].GetName())
    plt.legend()
    plt.grid()
    plt.xlabel('system size')
    plt.ylabel('relative performance')
    plt.show(block=False)


def PlotRelativePerformanceWithIndividualRef(benchmarkPairs, title):
    fig = plt.figure()
    for benchmarkPair in benchmarkPairs:
        benchmarkName = benchmarkPair[0].GetName()
        if len(benchmarkPair[0].size) == 0:
            print('No data for benchmark: ' + benchmarkPair[0].GetName())
            continue
        elif len(benchmarkPair[1].size) == 0:
            print('No data for benchmark: ' + benchmarkPair[1].GetName())
            continue
        else:
            time = copy.deepcopy(benchmarkPair[0].time)
            for i in range(len(time)):
                time[i] /= benchmarkPair[1].time[i]

            plt.plot(benchmarkPair[0].size, time, color=benchmarkPair[0].color, label=benchmarkName)

    plt.title(title)
    plt.legend()
    plt.grid()
    plt.xlabel('system size')
    plt.ylabel('relative performance')
    plt.show(block=False)


def PlotExecutionTime(benchmarks):
    fig = plt.figure()
    for benchmark in benchmarks:
        benchmarkName = benchmark.GetName()
        if len(benchmark.size) == 0:
            print('No data for benchmark: ' + benchmarkName)
            continue
        else:
            plt.plot(benchmark.size, benchmark.time, color=benchmark.color, label=benchmarkName)

    plt.title('Execution time')
    plt.legend()
    plt.show(block=False)


### -------------------------------------------------------------------------------------------------------------------
### Main
### -------------------------------------------------------------------------------------------------------------------


if runBenchmarks:
    RunBenchmarks()

timeScalingFactor = 1

gaussData = ReadBenchmarkResult('Gauss', gaussColor)
luData = ReadBenchmarkResult('LU', luColor)
qrData = ReadBenchmarkResult('QR', qrColor)

benchmarkResults = [ReadBenchmarkResult('Gauss', gaussColor), ReadBenchmarkResult('LU', luColor),
                    ReadBenchmarkResult('QR', qrColor)]

if compareSolvers:
    solverBenchmarks = []
    for benchmark in benchmarkResults:
        solverBenchmarks.append(benchmark.GetBenchmark(compareSolversSIMD, compareSolversDataType, compareSolversPivot))

    solverBenchmarksReferenceIndex = 0
    for i in range(len(solverBenchmarks)):
        if solverBenchmarks[i].solverName == compareSolversReferenceSolver:
            solverBenchmarksReferenceIndex = i
            break

    PlotRelativePerformance(solverBenchmarks, solverBenchmarksReferenceIndex)
    PlotExecutionTime(solverBenchmarks)

if compareSerialSimd:
    benchmarkPairs = []
    for benchmark in benchmarkResults:
        benchmarkPairs.append([benchmark.GetBenchmark(False, compareSerialSimdDataType, compareSerialSimdPivot),
                               benchmark.GetBenchmark(True, compareSerialSimdDataType, compareSerialSimdPivot)])

    PlotRelativePerformanceWithIndividualRef(benchmarkPairs, 'SIMD Speedup')

plt.show()
