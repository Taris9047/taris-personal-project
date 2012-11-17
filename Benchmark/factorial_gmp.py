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
	
	for i in range(1,N+1,1):
		if N == 1:
			return 1
		else:
			fact *= i
				
	return fact

N = 100000
startTime = time.clock()
fact = factorial(N)
endTime = time.clock()
print ("Calculation of %d! has been finished in %.3f seconds"%(N, endTime-startTime))
#f = open('./factorial_gmp.txt', 'w')
#f.write(str(fact))
#f.close()
#print ("result saved to file!!")