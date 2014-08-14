#!/usr/bin/env python

# Torture test script for T-Bench

import factorial as factN
import gfactorial as gfactN
import random
import time
import utils
import sys

import Benchmark as Bench

def torture(min_fact=1, max_fact=10000000):
	torture_log = open("Torture.log", "w")
	print ("  Hit Ctrl+C to break out...")
	print (" ")

	controller = 1
	i = 0
	N = []
	while controller:		
		random.seed()
		N.append(random.randrange(min_fact,max_fact))
		sys.setrecursionlimit(N[i]+10)
		
		torture_log = open("Torture.log", "a")

		print(("Calculating %d!"%N[i]))
		torture_log.write("%d!:\n"%N[i])
		factN_nogmp, time_elipsed = factN.factNa([N[i]])
		torture_log.write(\
			"[NOGMP] %d!\tfinished in %.6f seconds.\n"%(N[i],time_elipsed))
		 
		factN_gmp, gtime_elipsed = gfactN.factNa([N[i]])
		torture_log.write(\
			"[GMP] %d!\tfinished in %.6f seconds.\n"%(N[i],gtime_elipsed))


		if factN_nogmp == factN_gmp:
			print("Numbers match!! next...")
			print("[GMP] is %.2f times faster."%(time_elipsed/gtime_elipsed))
			torture_log.write(" %d! has been success!!\n"%N[i])
			torture_log.write("[GMP] is %.2f times faster.\n"%(time_elipsed/gtime_elipsed))
			torture_log.write("\n")
			torture_log.close()
			i += 1
		else:
			print("Uh oh... something is wrong... ")
			torture_log.write(" Ooops... %s! had problem...\n"%N[i])
			torture_log.close()
			raise ValueError(\
				"No gmp value: %d does not match with gmp value: %d !!"\
				%(factN_nogmp, factN_gmp))
		print (" ")

	return 0

def main():
	import multiprocessing as mp
	mp.freeze_support()
	
	print(("*** T-Bench (Torture Test Edition) ver. Abysmal "+Bench.ver_number+" ***"))
	print (" ")
	print ("    T-Bench (Torture Test Edition) Copyright (C) 2012  Taylor Shin.\n\
    This program comes with ABSOLUTELY NO WARRANTY.\n\
    This is free software, and you are welcome to redistribute it.\n")

	print((utils.sysinfo()))

	torture(1, 1000000)

	return 0

if __name__ == '__main__':
	main()