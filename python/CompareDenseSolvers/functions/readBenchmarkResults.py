
import json

### Classes -----------------------------------------------------------------------------------------------------------


class BenchmarkResults:
    def __init__(self, solverName, typeName, pivotName,color):
        self.solverName = solverName
        self.pivotName = pivotName
        self.typeName = typeName
        self.color = color
        self.size = list()
        self.time = list()
        self.cpu = list()

    def SetVectorizationName(self, vectorizationName):
        self.vectorizationName = vectorizationName
        self.benchmarkName = self.solverName + " - " + vectorizationName + " - " + self.typeName + " - " + self.pivotName
        self.benchmarkNameNoType = self.solverName + " - " + vectorizationName + " - " + self.pivotName
        self.benchmarkNameShort = self.solverName + " - " + self.pivotName

    def GetName(self):
        return self.solverName + " - " + self.vectorizationName + " - " + self.typeName + " - " + self.pivotName



class SolverBenchmarkData:

    def __init__(self, solverName, color):
        self.simd = lambda: 0
        self.simd.F32 = lambda: 0
        self.simd.F64 = lambda: 0
        self.simd.F32.noPivot = BenchmarkResults(solverName, 'F32', 'no pivoting', color)
        self.simd.F32.partialPivot = BenchmarkResults(solverName, 'F32', 'partial pivoting', color)
        self.simd.F64.noPivot = BenchmarkResults(solverName, 'F64', 'no pivoting', color)
        self.simd.F64.partialPivot = BenchmarkResults(solverName, 'F64', 'partial pivoting', color)
        self.serial = lambda: 0
        self.serial.F32 = lambda: 0
        self.serial.F64 = lambda: 0
        self.serial.F32.noPivot = BenchmarkResults(solverName, 'F32', 'no pivoting', color)
        self.serial.F32.partialPivot = BenchmarkResults(solverName, 'F32', 'partial pivoting', color)
        self.serial.F64.noPivot = BenchmarkResults(solverName, 'F64', 'no pivoting', color)
        self.serial.F64.partialPivot = BenchmarkResults(solverName, 'F64', 'partial pivoting', color)
        self.serial.F32.noPivot.SetVectorizationName('Serial')
        self.serial.F32.partialPivot.SetVectorizationName('Serial')
        self.serial.F64.noPivot.SetVectorizationName('Serial')
        self.serial.F64.partialPivot.SetVectorizationName('Serial')
        self.simd.F32.noPivot.SetVectorizationName('SIMD')
        self.simd.F32.partialPivot.SetVectorizationName('SIMD')
        self.simd.F64.noPivot.SetVectorizationName('SIMD')
        self.simd.F64.partialPivot.SetVectorizationName('SIMD')
        self.vectorizationNameSet = False

    def SetVectorizationName(self, vectorizationName):
        if not self.vectorizationNameSet:
            self.simd.F32.noPivot.SetVectorizationName(vectorizationName)
            self.simd.F32.partialPivot.SetVectorizationName(vectorizationName)
            self.simd.F64.noPivot.SetVectorizationName(vectorizationName)
            self.simd.F64.partialPivot.SetVectorizationName(vectorizationName)
            self.vectorizationNameSet = True

    def GetBenchmark(self, simd, typeName, pivotName):
        if simd:
            return self.__SelectType(self.simd, typeName, pivotName)
        else:
            return self.__SelectType(self.serial, typeName, pivotName)

    def __SelectType(self, benchmarkData, typeName, pivotName):
        assert (typeName == 'F32' or typeName == 'F64')

        if typeName == 'F32':
            return self.__SelectPivot(benchmarkData.F32, pivotName)
        else:
            return self.__SelectPivot(benchmarkData.F64, pivotName)

    def __SelectPivot(self, benchmarkData, pivotName):
        assert (pivotName == 'NoPivot' or pivotName == 'PartialPivot')

        if pivotName == 'NoPivot':
            return benchmarkData.noPivot
        elif pivotName == 'PartialPivot':
            return benchmarkData.partialPivot


### Functions ---------------------------------------------------------------------------------------------------------


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
    bmResults.time.append(float(bmData['real_time']))
    bmResults.cpu.append(float(bmData['cpu_time']))


def ReadBenchmarkResult(solverName, color):
    solverBenchmarkData = SolverBenchmarkData(solverName, color)
    fileName = solverName + '.json'

    with open(fileName) as json_file:
        fileData = json.load(json_file)
        for bm in fileData['benchmarks']:
            [simd, typeString, vectorizationString, pivotingString, size] = ExtractBenchmarkSetupFromName(bm['name'])
            if simd:
                solverBenchmarkData.SetVectorizationName(vectorizationString)

            benchmarkData = solverBenchmarkData.GetBenchmark(simd, typeString, pivotingString)
            AppendBenchmarkResults(benchmarkData, bm, size)

    return solverBenchmarkData