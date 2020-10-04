import sys
import os
import numpy as np
import matplotlib.pyplot as plt


res = np.loadtxt('simpleData.txt')
nqubit = res.T[2]
durations = res.T[4]
realMems = res.T[5]
virtMems = res.T[6]

plt.title("Duration vs NumQubits")

fig, ax1 = plt.subplots()
line, = plt.plot(nqubit, realMems, '--bo', lw=2, label='2.7 GHz Dual-Core Intel Core i5, #Threads=1')
ax1.legend(handles=[line,])
ax1.set_xlabel('Num of Qubits', fontsize=14)
ax1.set_ylabel('Real Memory/Bytes', fontsize=14)

plt.tight_layout()
plt.savefig('realMem_vs_qubit.pdf', format='pdf', dpi=1000)
