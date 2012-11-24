#!/usr/bin/env python

# Contains utility functions

import platform as plf
import os, subprocess, re

# Probing CPU type depending on current OS
def cpu_type(OS_type):
	# Detecting IronPython
	import sys
	if '.NET' in sys.version:
		__IronPython__ = True

	if OS_type == 'Windows' or __IronPython__ == True:
		import _winreg
		key = getattr(_winreg, "HKEY_LOCAL_MACHINE")
		handle = _winreg.OpenKey(key, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0")
		value, type = _winreg.QueryValueEx(handle, "ProcessorNameString")
		return value
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
		unknown_OS_msg = \
		 '* Current OS Type: '+str(OS_type)+' is detected. Unable to detect CPU type on that platform for now...'
		return unknown_OS_msg
	return 0

# grabs system information
def sysinfo():
	#operating_system = plf.platform()
	uname = plf.uname()
	py_info = 'Python '+\
			str(plf.python_version())+' '+\
			str(plf.python_compiler())
	return py_info+'\n'+'CPU Type: '+\
			str(cpu_type(uname[0]))+'\n'+str(uname[3])+'\n'

# saves everything into file
def savelist(savefile, save_table, System_Info='__default__'):
	# Update System_Info automatically unless manually set.
	if System_Info == '__default__':
		System_Info = sysinfo()
	else:
		System_Info = '** Custom System Info:\n'+System_Info
	
	f = open(savefile, 'w')
	f.write(System_Info)
	f.write('\n')
	f.write("N\tChunks(Processes)\tElipsed Time(sec)\n")
	for i in range(len(save_table)):
		if i == len(save_table[i])-1:
			f.write(save_table[i])
		else:
			f.write(save_table[i]+"\n")
	f.close()

	return savefile

