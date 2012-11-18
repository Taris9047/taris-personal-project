# Factorial lib with GMPY2 addition
#
import gmpy2 as gmp

def factorial(N):
	fact = gmp.mpz(1)
	
	for i in range(1,N+1,1):
		if N == 1:
			return 1
		else:
			fact *= i
				
	return fact

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
