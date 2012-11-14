#!/usr/bin/env python
# Currently, divide and conquer is not working on Python 2.7.3 OS X 
# due to recursion limit and stack overflow.
# 

import sys
import time

def factorial_dnc(S, E):
	if S == E:
		return S
	else:
		return E * factorial_dnc(S, E-1)

def dnc(N):
	seg_range = 10000
	divide_num = N/seg_range

	sys.setrecursionlimit(seg_range+10)

	fact = 1
	for i in range(1, divide_num+1):
		start = ((i-1)*seg_range)+1
		end = i+seg_range
		fact *= factorial_dnc(start, end)
	return fact

N = 100000
startTime = time.clock()
factorial = dnc(N)
endTime = time.clock()

print ("Calculation of %d! has been finished in %s seconds"%(N, endTime-startTime))
print factorial