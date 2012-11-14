#!/usr/bin/env python
import sys
import time

def factorial(N):
	fact = 1
	# Setting up toolbar
	toolbar_width = 40
	sys.stdout.write("[%s]"%(" " * toolbar_width))
	sys.stdout.flush()
	sys.stdout.write("\b"*(toolbar_width+1))

	for i in range(1,N+1,1):
		if N == 1:
			fact = 1
		else:
			fact = fact*i
			toolbar_unit = float(N)/toolbar_width
			if float(i)%toolbar_unit == 0:
				sys.stdout.write("=")
				sys.stdout.flush()

	sys.stdout.write("\n")
	return fact

N = 100000
startTime = time.clock()
fact = factorial(N)
endTime = time.clock()
print ("Calculation of %d! has been finished in %.3f seconds"%(N, endTime-startTime))