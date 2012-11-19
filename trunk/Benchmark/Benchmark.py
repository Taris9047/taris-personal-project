#!/usr/bin/env python
#
# Factorial benchmark master file
# requires GMPY2 package to run gmp routines
#

import time
import sys
import factor_lib as flib
import platform as plf
import cpuinfo as cpu

# Saving up information
operating_system = plf.platform()
uname = plf.uname()
System_Info = cpu.cpu_type(uname[0])+'\n'+uname[3]+"\n"

# Running routines
#N = [1000, 5000, 10000, 50000, 100000, 500000]
N = [1000, 5000, 10000, 50000, 100000]
print ("Calculation range: ",N)
time_diff = [None] * len(N)
factN = []
factNdnc = []

print (System_Info)
print ("Calculating Generic Sequential Algorithm...")
for i in range(len(N)):
	startTime = time.clock()
	factN = flib.factorial(N[i])
	endTime = time.clock()
	time_diff[i] = endTime-startTime
	print ("%d!	has been finished in %.6f seconds."%(N[i], time_diff[i]))

sequential_performance = time_diff
f = open("Sequential.txt", 'w')
f.write(System_Info)
f.write("N\tElipsed Time\n")
for i in range(len(N)):
	f.write(str(N[i])+"\t"+str(sequential_performance[i])+"\n")
f.close()

print (" ")
print ("Calculating Divide and Conquer Algorithm...")
for i in range(len(N)):
	startTime = time.clock()
	factNdnc = flib.dnc(N[i],N[i]/100)
	endTime = time.clock()
	time_diff[i] = endTime-startTime
	print ("%d!	has been finished in %.6f seconds."%(N[i], time_diff[i]))

dnc_performance = time_diff
f = open("DNC.txt", 'w')
f.write(System_Info)
f.write("N\tElipsed Time\n")
for i in range(len(N)):
	f.write(str(N[i])+"\t"+str(dnc_performance[i])+"\n")
f.close()
