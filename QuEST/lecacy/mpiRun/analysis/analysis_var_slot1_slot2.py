

import numpy as np

numQubits = 20
nThreads = 1
nRepetition = 1
cdepth = 100


slot1_array = np.array([1,2,4,8])
dur_array = np.zeros(len(slot1_array))
realMem_array = np.zeros(len(slot1_array))
realPeak_array = np.zeros(len(slot1_array))
virtMem_array = np.zeros(len(slot1_array))
virtPeak_array = np.zeros(len(slot1_array))

for si, slot1 in enumerate(slot1_array):
    slot2 = slot1
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

    dur_array[si] = dur_max
    realMem_array[si] = realMem_avg
    realPeak_array[si] = realPeak_avg
    virtMem_array[si] = virtMem_avg
    virtPeak_array[si] = virtPeak_avg


import matplotlib.pyplot as plt

plt.plot(slot1_array, dur_array,'-o')
plt.xlabel("#slot1 & slot2")
plt.ylabel("time/s")
title = "Time vs Slot1 & Slot2\nnumThread=%d, numQubits=%d"%(nThreads, numQubits)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)


plt.figure()
plt.plot(slot1_array, realMem_array,'-o')
plt.xlabel("#slot1 & slot2")
plt.ylabel("realMem/B")
title = "realMem vs Slot1 & Slot2\nnumThread=%d, numQubits=%d"%(nThreads, numQubits)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)

plt.figure()
plt.plot(slot1_array, realPeak_array,'-o')
plt.xlabel("#slot1 & slot2")
plt.ylabel("realMemPeak/B")
title = "realPeak vs Slot1 & Slot2\nnumThread=%d, numQubits=%d"%(nThreads, numQubits)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)

plt.figure()
plt.plot(slot1_array, virtMem_array,'-o')
plt.xlabel("#slot1 & slot2")
plt.ylabel("virtMem/B")
title = "virtMem vs Slot1 & Slot2\nnumThread=%d, numQubits=%d"%(nThreads, numQubits)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)

plt.figure()
plt.plot(slot1_array, virtPeak_array,'-o')
plt.xlabel("#slot1 & slot2")
plt.ylabel("virtMemPeak/B")
title = "virtPeak vs Slot1 & Slot2\nnumThread=%d, numQubits=%d"%(nThreads, numQubits)
plt.title(title)
plt.savefig("../result_graphs/%s"%title)





