#!/usr/bin/env python

# Contains utility functions

# grabs system information
import platform as plf
import os, subprocess, re

# Probing CPU type depending on current OS
def cpu_type(OS_type):
    if OS_type == 'Windows':
        return plf.processor()
    elif OS_type == 'Darwin':
        import os
        os.environ['PATH'] = os.environ['PATH'] + os.pathsep + '/usr/sbin'
        command ="sysctl -n machdep.cpu.brand_string"
        return subprocess.check_output(command, shell=True).strip()
    elif OS_type == 'Linux':
        command = "cat /proc/cpuinfo"
        all_info = subprocess.check_output(command, shell=True).strip()
        for line in allInfo.split("\n"):
            if "model name" in line:
                return re.sub( ".*model name.*:", "", line,1)
    else:
    	return 'Unknown Processor'
    return ""

def sysinfo():
	#operating_system = plf.platform()
	uname = plf.uname()
	py_info = 'Python '+\
			str(plf.python_version())+' '+\
			str(plf.python_compiler())
	return py_info+'\n'+'CPU Type: '+\
			str(cpu_type(uname[0]))+'\n'+str(uname[3])+'\n'

# saves everything into file
def savelist(savefile, list_data, System_Info='__default__'):
	# Update System_Info automatically unless manually set.
	if System_Info == '__default__':
		System_Info = sysinfo()
	else:
		System_Info = '** Custom System Info:\n'+System_Info
	
	f = open(savefile, 'w')
	f.write(System_Info)
	f.write('\n')
	f.write("N\tElipsed Time\n")
	for i in range(len(list_data)):
		for j in range(len(list_data[i])):
			if j == len(list_data[i])-1:
				f.write(str(list_data[i][j])+"\n")
			else:
				f.write(str(list_data[i][j])+"\t")
	f.close()
	return savefile

# re-arranging data
def arrange_data(N, time_diff):
	arranged_list = [None] * len(N)
	for i in range(len(N)):
		arranged_list[i] = [ str(N[i]), str(time_diff[i]) ]
	return arranged_list