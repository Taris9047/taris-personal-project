#!/usr/bin/env python

# Contains utility functions

import platform as plf
import os, subprocess, re

# Detecting IronPython
import sys

# Probing CPU type depending on current OS
def cpu_type(OS_type):

	if OS_type == 'Windows':
		if plf.python_version().split('.')[0] == '2':
			import _winreg
		elif plf.python_version().split('.')[0] == '3':
			import winreg as _winreg
		else:
			return "Unable to read CPU type from registry."

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
		for line in all_info.split("\n"):
			if "model name" in line:
				return re.sub( ".*model name.*:", "", line,1)
	else:
		unknown_OS_msg = \
		 '* Current OS Type: '+str(OS_type)+' is detected. Unable to detect CPU type on that platform for now...'
		return unknown_OS_msg
	return 0

# grabs system information
def sysinfo(verbose=False):
	#operating_system = plf.platform()
	uname = plf.uname()
	py_info = 'Python '+\
			str(plf.python_version())+' '+\
			str(plf.python_compiler())
	sys_info_text = py_info+'\n'+'CPU Type: '+\
			str(cpu_type(uname[0]))+'\n'+str(uname[3])+'\n'
	if verbose == True:
		print sys_info_text
	
	return sys_info_text


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


# parsing arguments
# Available options:
# all, seq, dnc, dnc_m, auto, 
# gseq, gdnc, gdnc_m, gauto, and torture_test
#
def bench_options(arg_list):
	if len(arg_list) == 1:
		return ['seq', 'dnc', 'dnc_m', 'auto', \
				'gseq', 'gdnc', 'gdnc_m', 'gauto'],\
				[]
	else:
		bench_option_list = []
		bench_number_list = []
		arg_list = arg_list[1:len(arg_list)]
		arg_list = list(set(arg_list))
		
		for arg in arg_list:
			if arg == 'all':
				bench_option_list = \
				['seq', 'dnc', 'dnc_m', 'auto', \
				'gseq', 'gdnc', 'gdnc_m', 'gauto']
				break
			elif arg.isdigit() == True:
				bench_number_list.append(int(arg))
			elif arg == 'seq' or \
				arg == 'dnc' or \
				arg == 'dnc_m' or \
				arg == 'auto' or \
				arg == 'gseq' or \
				arg == 'gdnc' or \
				arg == 'gdnc_m' or \
				arg == 'gauto':
				bench_option_list.append(arg)
			elif arg == 'torture':
				bench_option_list = ['torture']
				break
			else:
				print ("Invalid option: %s"%(arg))
				print ("Ignoring it")
				pass
		
		# removing cruds
		bench_option_list = list(set(bench_option_list))
		bench_number_list = list(set(bench_number_list))

		return bench_option_list, bench_number_list

# print calculation range
def print_range(N):
	N_str = ''
	for n in N:
		N_str += str(n)+', '

	print ("Calculation Range: %s"%N_str)

	return ''