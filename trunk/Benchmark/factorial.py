#!/usr/bin/env python
# Typical factorial script with a touch of progress bar.
# Non-recursive.
import sys
import time

def factorial(N):
	fact = 1

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
#print fact
#f = open('./factorial.txt', 'w')
#f.write(str(fact))
#f.close()
#print ("result saved to file!!")