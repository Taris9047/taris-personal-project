#!/usr/bin/env python
import sys
import utils
import platform
from datetime import date
# importing for version name
import Benchmark as Bench
from cx_Freeze import setup, Executable

# Setting up target directory
prog_name = 'T-Bench'
build_dir = 'build'
OS_name = str(sys.platform)
#Python_name = platform.python_implementation()
#Python_version = platform.python_version()
if utils.is32bit() == True:
	architecture = '32bit'
elif utils.is32bit() == False:
	architecture = '64bit'
else:
	architecture = 'unknown'

today = date.today()
compile_date = str(today.month)+str(today.day)+str(today.year)
#dir_name = './'+build_dir+'/'+ \
#			prog_name+'-'+Bench.ver_number+'-'+ \
#			OS_name+'-'+architecture+'-'+ \
#			compile_date

# Dependencies are automatically detected, but it might need fine tuning.
packages = ["os","sys","time","gmpy2","platform",\
	"multiprocessing","subprocess","re","random"]
optimize = 2

#build_exe_options = {"build_exe": dir_name, \
#					"packages": packages, \
#					"optimize": optimize,
#					"compressed": True}

build_exe_options = {"packages": packages, \
					"optimize": optimize,
					"compressed": True}


# GUI applications require a different base on Windows (the default is for a
# console application).
#base = None
#if sys.platform == "win32":
#    base = "Win32GUI"

setup(  name = "T-Bench",
        version = Bench.ver_number,
        description = "T-Bench: An abysmal factorial calculation benchmark!!",
        author = 'Taylor Shin',
        author_email = 'k4shin@uwaterloo.ca',
        options = {'build_exe':build_exe_options},
        executables = [Executable("Benchmark.py"), Executable("Torture.py")])




