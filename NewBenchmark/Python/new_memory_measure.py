
_FIELDS = ['VmRSS', 'VmHWM', 'VmSize', 'VmPeak']

import os

def sizeStirngToInteger(sizeString):
    unit = sizeString[-1]
    value = sizeString[:-1]
    if unit == "K":
        return int(float(value)*1024)
    elif unit == "M":
        return int(float(value)*1024*1024)
    elif unit == "G":
        return int(float(value)*1024*1024*1024)
    else:
        return int(float(value))


def get_memory():
    '''
    returns the current and peak, real and virtual memories
    used by the calling linux python process, in Bytes
    '''

    pid = os.getpid()
    cmd = "vmmap %d -summary > memoryFile"%pid
    os.system(cmd)

    try:
        with open("memoryFile", "r") as file:
            for line in file:
                linesplit = line.split()
                if len(linesplit) > 0 and linesplit[0] == "TOTAL":
                    virtSize = linesplit[1]
                    realSize = linesplit[2]
    except:
        print("Call to getMemory FAILDED; mac file memoryFile not found!\n")
        return 1

    mem = {}
    mem["VmRSS"] = sizeStirngToInteger(realSize)
    mem["VmSize"] = sizeStirngToInteger(virtSize)
    mem["VmHWM"] = sizeStirngToInteger(realSize)
    mem["VmPeak"] = sizeStirngToInteger(virtSize)
    return mem


if __name__ == '__main__':

    # a simple test
    print(get_memory())
    mylist = [1.5]*2**10  # 30
    print(get_memory())
    del mylist
    print(get_memory())
