import sys
import matplotlib.pyplot as plt

sys.path.append('functions')

from runBenchmarks import *
from readBenchmarkResults import *

### -------------------------------------------------------------------------------------------------------------------
### Setup
### -------------------------------------------------------------------------------------------------------------------


runBenchmarks = False

### -------------------------------------------------------------------------------------------------------------------
### Main
### -------------------------------------------------------------------------------------------------------------------


if runBenchmarks:
    RunBenchmarks()

timeScalingFactor = 1

gaussData = ReadBenchmarkResult('Gauss')
luData = ReadBenchmarkResult('LU')
qrData = ReadBenchmarkResult('QR')

plt.plot(luData.simd.F32.noPivot.size, luData.simd.F32.noPivot.time, label=luData.simd.F32.noPivot.benchmarkName)
plt.plot(gaussData.simd.F32.noPivot.size, gaussData.simd.F32.noPivot.time,
         label=gaussData.simd.F32.noPivot.benchmarkName)
# plt.plot(luData.serial.F32.noPivot.size, luData.serial.F32.noPivot.time, label=luData.serial.F32.noPivot.benchmarkName)
# plt.plot(gaussData.serial.F32.noPivot.size, gaussData.serial.F32.noPivot.time,
#         label=gaussData.serial.F32.noPivot.benchmarkName)
# plt.plot(qrData.serial.F32.noPivot.size, qrData.serial.F32.noPivot.time,
#         label=qrData.serial.F32.noPivot.benchmarkName)

# plt.plot(luData.serial.F64.noPivot.size, luData.serial.F64.noPivot.time, label=luData.serial.F64.noPivot.benchmarkName)
# plt.plot(gaussData.serial.F64.noPivot.size, gaussData.serial.F64.noPivot.time,
#         label=gaussData.serial.F64.noPivot.benchmarkName)
# plt.plot(qrData.serial.F64.noPivot.size, qrData.serial.F64.noPivot.time,
#         label=qrData.serial.F64.noPivot.benchmarkName)

# plt.plot(luData.simd.F64.noPivot.size, luData.simd.F64.noPivot.time, label=luData.simd.F64.noPivot.benchmarkName)
# plt.plot(luData.simd.F32.partialPivot.size, luData.simd.F32.partialPivot.time, label=luData.simd.F32.partialPivot.benchmarkName)
# plt.plot(luData.serial.F32.noPivot.size, luData.serial.F32.noPivot.time)
# plt.plot(luData.serial.F32.partialPivot.size, luData.serial.F32.partialPivot.time)
plt.legend()

plt.show()
