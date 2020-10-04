import sys

pid = sys.argv[1]
rank = sys.argv[2]
name = 'stat_'+pid

fstat = open(name,"r")
for line in fstat:
	linesplit = line.split(" ")
	utime = int(linesplit[13])
	stime = int(linesplit[14])
	cutime = int(linesplit[15])
	cstime = int(linesplit[16])
	starttime = int(linesplit[21])
	cpu_id = int(linesplit[38])
Hertz = 100.
fstat.close()


fup = open("uptime_%s"%pid,"r")
for line in fup:
	linesplit = line.split(" ")
	uptime = float(linesplit[0])
fup.close()


total_time = utime + stime
total_time = total_time + cutime + cstime
seconds = uptime - (starttime / Hertz)
cpu_usage = 100. * ((total_time / Hertz) / seconds)

# print("u_time: {}".format(utime)) 
# print("s_time: {}".format(stime)) 
# print("cu_time: {}".format(cutime)) 
# print("cs_time: {}".format(cstime)) 
# print("starttime: {}".format(starttime)) 
# print("uptime: {}".format(uptime)) 
# print("total_time: {}".format(total_time)) 
# print("cpu_id: {}".format(cpu_id))
# print("cpu_usage: {} %".format(cpu_usage))

result = "rank: %s, cpu_id: %d, pid: %s, cpu_usage: %.5f %%\n"%(rank, cpu_id, pid, cpu_usage)
with open("result_%s.txt"%rank, "a+") as fres:
	fres.write(result)

