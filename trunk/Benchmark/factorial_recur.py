#!/usr/bin/env python
# May not run on huge numbers since Python does not support
# such large recursion out of the box. If an extensive recursion limit
# was enforced, the interpreter emits 'Stack Overflow'
# 
# Thus, it is recommended to use 'Stackless Python' to run this code...
#
import sys
import time

def factorial(N):
	if N == 1:
		return 1
	else:
		return N * factorial(N-1)

N = 50000
sys.setrecursionlimit(N+10)
startTime = time.clock()
fact = factorial(N)
endTime = time.clock()

print ("Calculation of %d! has been finished in %s seconds"%(N, endTime-startTime))
print fact