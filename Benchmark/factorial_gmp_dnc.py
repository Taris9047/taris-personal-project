#!/usr/bin/env python
# Sequencial dnc routine implemented due to the stack overflow problem.
# However, since everything happens by sequence... I'm not even sure
# this is actually the 'divide and conquer.'
#
# However, this code shows dramatical performance increase!!
# 

import sys
import time

# Loading gmp library
print "Loading GMPY2 package..."
import gmpy2 as gmp

def mul_list(RNG):
	if len(RNG) == 1:
		return gmp.mpz(RNG[0])
	else:
		fact = gmp.mpz(1)
		for i in range(len(RNG)):
			fact *= RNG[i]
		return fact

def dnc(N, threads):
	if threads > N:
		raise ValueError("Threads value %d is larger than computation parameter %d."%(threads,N))
		exit(-1)
	else:
		# Generating Parts
		part_len = N/threads
		N_list = range(1, N+1, 1)
		factRNG = [N_list[i:i+part_len] for i in range(0, N, part_len)]
		N_list = []

		fact_result = gmp.mpz(1)
		for i in range(len(factRNG)):
			fact_result *= mul_list(factRNG[i])
			factRNG[i] = []

		return fact_result

N = 100000
startTime = time.clock()
fact = dnc(N,100)
endTime = time.clock()

print ("Calculation of %d! has been finished in %s seconds"%(N, endTime-startTime))
#print fact
# Print result to file
#f = open('./factorial_dnc.txt', 'w')
#f.write(str(fact))
#f.close()
#print ("result saved to file!!")


