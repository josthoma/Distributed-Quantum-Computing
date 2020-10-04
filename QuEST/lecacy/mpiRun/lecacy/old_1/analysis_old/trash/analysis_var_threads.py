

import numpy as np

numQubits = 20
slot1 = 1
slot2 = 0
nRepetition = 1
cdepth = 100

minNumThread = 1
maxNumThread = 12
nThread_array = np.array(range(minNumThread, maxNumThread))

dur_array = np.zeros(maxNumThread - minNumThread + 1)
realMem_array = np.zeros(maxNumThread - minNumThread + 1)
realPeak_array = np.zeros(maxNumThread - minNumThread + 1)
virtMem_array = np.zeros(maxNumThread - minNumThread + 1)
virtPeak_array = np.zeros(maxNumThread - minNumThread + 1)

for index, nThread in enumerate(nThread_array):
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

    dur_array[index] = dur_max
    realMem_array[index] = realMem_avg
    realPeak_array[index] = realPeak_avg
    virtMem_array[index] = virtMem_avg
    virtPeak_array[index] = virtPeak_avg



# ============================================================================
# ================================== Plot ====================================
# ============================================================================

import matplotlib.pyplot as plt

xlabel = "#Thread"
ylabel_array = ["time/s","realMem/B", "realMemPeak/B", "virtMem/B", "virtMemPeak/B"]
title_array = ["Time", "realMem", "realMemPeak", "virtMem", "virtMemPeak"]

xaxis = nThread_array
yaxis_array = [dur_array, realMem_array, realPeak_array, virtMem_array, virtPeak_array]

for i in range(5):
    plt.figure()
    plt.plt(xaxis, yaxis_array[i], '-o')
    plt.xlabel(xlabel)
    plt.ylabel(ylabel_array[i])
    title = title_array + " vs Slot1\nnumThread=%d, slot2=%d, numQubits=%d"%(nThreads, slot2, numQubits)
    plt.savefig("../result_graphs/%s"%title)















