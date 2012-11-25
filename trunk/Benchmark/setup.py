import sys
from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but it might need fine tuning.
build_exe_options = {"packages": \
	["os","sys","time","multiprocessing","subprocess","re"]}

# GUI applications require a different base on Windows (the default is for a
# console application).
#base = None
#if sys.platform == "win32":
#    base = "Win32GUI"

setup(  name = "T-Bench",
        version = "0.0.4.2",
        description = "An abysmal factorial calculation benchmark!!",
        executables = [Executable("Benchmark.py")])