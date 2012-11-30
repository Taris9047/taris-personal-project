#!/usr/bin/env python

# Torture test script for T-Bench

import factorial as factN
import gfactorial as gfactN
import random
import time
import utils
import sys

import Benchmark as Bench
ver_number = Bench.ver_number

def main():
	import multiprocessing as mp
	mp.freeze_support()

	print ("*** T-Bench (Torture Test Edition) ver. Abysmal "+ver_number+" ***")
	print (" ")
	print ("    T-Bench (Torture Test Edition) Copyright (C) 2012  Taylor Shin.\n\
    This program comes with ABSOLUTELY NO WARRANTY.\n\
    This is free software, and you are welcome to redistribute it.\n")

	print (utils.sysinfo())

	controller = 1
	while controller:
		print ("  Hit Ctrl+C to break out...")
		print (" ")
		random.seed()
		rand_min = 100
		rand_max = 10000000
		N = [random.randrange(rand_min,rand_max)]
		print ("Calculating %d!"%N[0])
		print ("Without GMP")
		factN_nogmp = factN.factNa(N)
		print ("With GMP")
		factN_gmp = gfactN.factNa(N)
		
		if factN_nogmp == factN_gmp:
			print("Numbers match!! next...")
		else:
			print("Uh oh... something is wrong... ")
			print("No gmp value: %d does not match with gmp value: %d !!"\
				%(factN_nogmp, factN_gmp))
			exit(1)
		print (" ")

	return 0

if __name__ == '__main__':
	main()