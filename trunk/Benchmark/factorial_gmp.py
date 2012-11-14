#!/usr/bin/env python
import sys
import time

# Requires Python GMP implementation
# Download it by typing:
# pip install gmpy
print "Loading GMPY2 package..."
import gmpy2 as gmp


def factorial(N):
	fact = gmp.mpz(1)
	
	# Setting up toolbar
	toolbar_width = 40
	sys.stdout.write("[%s]"%(" " * toolbar_width))
	sys.stdout.flush()
	sys.stdout.write("\b"*(toolbar_width+1))

	for i in range(1,N+1,1):
		if N == 1:
			return 1
		else:
			fact *= i
			toolbar_unit = float(N)/toolbar_width
			if float(i)%toolbar_unit == 0:
				sys.stdout.write("=")
				sys.stdout.flush()

	return fact
	sys.stdout.write("\n")

N = 100000
startTime = time.clock()
fact = factorial(N)
endTime = time.clock()
print ("Calculation of %d! has been finished in %.3f seconds"%(N, endTime-startTime))
