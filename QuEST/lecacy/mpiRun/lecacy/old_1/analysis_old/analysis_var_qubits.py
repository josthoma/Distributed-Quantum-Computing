

import numpy as np

minNumQubits = 1
maxNumQubits = 25
slot1 = 1
slot2 = 1
nThreads = 1
nRepetition = 1
cdepth = 100


qubit_array = np.array(range(minNumQubits, maxNumQubits+1))
dur_array = np.zeros(maxNumQubits - minNumQubits+1)
realMem_array = np.zeros(maxNumQubits - minNumQubits+1)
realPeak_array = np.zeros(maxNumQubits - minNumQubits+1)
virtMem_array = np.zeros(maxNumQubits - minNumQubits+1)
virtPeak_array = np.zeros(maxNumQubits - minNumQubits+1)

for numQubits in range(minNumQubits, maxNumQubits+1):
    filename = "../results/result_nqubits_%d_"%numQubits+\
               "slot1_%d_"%slot1+\
                "slot2_%d_"%slot2+\
                "nthreads_%d_"%nThreads+\
                "cdepth_%d_"%cdepth+\
                "nrepetition_%d"%nRepetition

    rank = np.array([0]*(slot1+slot2)*nRepetition).astype("int")
    dur = np.array([0]*(slot1+slot2)*nRepetition).astype("float")
    error = np.array([0]*(slot1+slot2)*nRepetition).astype("float")
    realMem = np.array([0]*(slot1+slot2)*nRepetition).astype("int")
    realPeak = np.array([0]*(slot1+slot2)*nRepetition).astype("int")
    virtMem = np.array([0]*(slot1+slot2)*nRepetition).astype("int")
    virtPeak = np.array([0]*(slot1+slot2)*nRepetition).astype("int")

    with open(filename, "r") as fid:
        for i, line in enumerate(fid):
            line = line.replace("rank: ","")
            line = line.replace("dur: ","")
            line = line.replace("error: ","")
            line = line.replace("realMem: ","")
            line = line.replace("(peak: ",",")
            line = line.replace("b)","")
            line = line.replace("virtMem: ","")
            line = line.replace("b","")
            line = line.replace("s","")
            linesplit = line.split(",")

            rank[i] = int(linesplit[0])
            dur[i] = float(linesplit[1])
            error[i] = float(linesplit[2])
            realMem[i] = int(linesplit[3])
            realPeak[i] = int(linesplit[4])
            virtMem[i] = int(linesplit[5])
            virtPeak[i] = int(linesplit[6])

    dur_max = max(dur)
    error_max = max(error)
    realMem_avg = np.average(realMem)
    realPeak_avg = np.average(realPeak)
    virtMem_avg = np.average(virtMem)
    virtPeak_avg = np.average(virtPeak)

    dur_array[numQubits - minNumQubits] = dur_max
    realMem_array[numQubits - minNumQubits] = realMem_avg
    realPeak_array[numQubits - minNumQubits] = realPeak_avg
    virtMem_array[numQubits - minNumQubits] = virtMem_avg
    virtPeak_array[numQubits - minNumQubits] = virtPeak_avg


import matplotlib.pyplot as plt

plt.plot(qubit_array, dur_array, '-o')
plt.xlabel("#qubits")
plt.ylabel("time/s")
title = "Time vs Qubits\nnumThread=%d, slot1=%d, slot2=%d"%(nThreads, slot1, slot1)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)


plt.figure()
plt.plot(qubit_array, realMem_array, '-o')
plt.xlabel("#qubits")
plt.ylabel("realMem/B")
title = "realMem vs Qubits\nnumThread=%d, slot1=%d, slot2=%d"%(nThreads, slot1, slot1)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)

plt.figure()
plt.plot(qubit_array, realPeak_array, '-o')
plt.xlabel("#qubits")
plt.ylabel("realMemPeak/B")
title = "realPeak vs Qubits\nnumThread=%d, slot1=%d, slot2=%d"%(nThreads, slot1, slot1)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)

plt.figure()
plt.plot(qubit_array, virtMem_array, '-o')
plt.xlabel("#qubits")
plt.ylabel("virtMem/B")
title = "virtMem vs Qubits\nnumThread=%d, slot1=%d, slot2=%d"%(nThreads, slot1, slot1)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)

plt.figure()
plt.plot(qubit_array, virtPeak_array, '-o')
plt.xlabel("#qubits")
plt.ylabel("virtMemPeak/B")
title = "virtPeak vs Qubits\nnumThread=%d, slot1=%d, slot2=%d"%(nThreads, slot1, slot1)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)









