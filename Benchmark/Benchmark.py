#!/usr/bin/env python
#
# Factorial benchmark master file
# requires GMPY2 package to run gmp routines
#

import time
import sys
import factor_lib as flib

# Running routines
N = [1000, 5000, 10000, 50000, 100000, 500000, 1000000]
print ("Calculation range: ",N)
time_diff = [None] * len(N)
threads = 10000
factN = []
factNdnc = []

print ("Calculating Generic Sequential Algorithm...")
for i in range(len(N)):
	startTime = time.clock()
	factN = flib.factorial(N[i])
	endTime = time.clock()
	time_diff[i] = endTime-startTime
	print ("%d!	has been finished in %.2f seconds."%(N[i], time_diff[i]))

sequential_performance = time_diff

print (" ")
print ("Calculating Divide and Conquer Algorithm...")
for i in range(len(N)):
	startTime = time.clock()
	factNdnc = flib.dnc(N[i],threads)
	endTime = time.clock()
	time_diff[i] = endTime-startTime
	print ("%d!	has been finished in %.2f seconds."%(N[i], time_diff[i]))

dnc_performance = time_diff
