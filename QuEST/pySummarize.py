
import re
import numpy as np

nqubits = 10
tran_dict = [{}, {}]
calc_dict = [{}, {}]
for cdepth in range(nqubits):
    tran_dict[0][cdepth] = []
    tran_dict[1][cdepth] = []
    calc_dict[0][cdepth] = []
    calc_dict[1][cdepth] = []
    fid = open(
        './results/result_nqubits_%d_slot1_1_slot2_1_cdepth_%d_nrepetition_1' % (nqubits, cdepth))
    for line in fid:
        linesplit = re.split(r',\s|:\s|\n', line)
        if linesplit[0] == 'rank':
            if linesplit[2] == 'transmission time':
                rank = int(linesplit[1])
                time = float(linesplit[3])
                tran_dict[rank][cdepth].append(time)
            elif linesplit[2] == 'calculation time':
                rank = int(linesplit[1])
                time = float(linesplit[3])
                calc_dict[rank][cdepth].append(time)
            elif linesplit[2] == 'transmission':  # specific for non-adaptive
                rank = int(linesplit[1])
                tran_dict[rank][cdepth].append(0)

print(tran_dict)
print(calc_dict)
