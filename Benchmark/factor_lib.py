# Factorial calculation library
#

import multiprocessing as mp
import sys

#
# Direct return
# In case of very small number... why calculate?
def fact_trivial(N):
	factN = [1, 1, 2, 6, 24, 120, 720, 5040, \
		40320, 362880, 3628800, 39916800, 479001600]
	if N < 0:
		print("Seriousely, just multiply -1!!")
		raise ValueError("%d is negative!!"%N)
	elif N > 12:
		print("Oops, too large for this function!!")
		raise ValueError("%d is too large!!"%N)
	else:
		return factN[N]

def fact_trivial64(N):
	factN = [1, 1, 2, 6, 24, 120, 720, 5040, \
		40320, 362880, 3628800, 39916800, 479001600, \
		6227020800, 87178291200, 1307674368000, 20922789888000, \
		355687428096000, 6402373705728000, 121645100408832000, \
		2432902008176640000]
	if N < 0:
		print("Seriousely, just multiply -1!!")
		raise ValueError("%d is negative!!"%N)
	elif N > 20:
		print("Oops, too large for this function!!")
		raise ValueError("%d is too large!!"%N)
	else:
		return factN[N]

#
# Sequential algorithm. 
# Very prehistoric but working.
#
def seq_fact(N):
	fact = 1

	for i in range(1,N+1,1):
		if N == 1:
			return 1
		else:
			fact *= i

	#print fact
	return fact

#
# Recursive algorithm...
# Doesn't work due to stack limitation.
#
def rseq_fact(N):
	if N == 1:
		return 1
	else:
		return N * rseq_fact (N-1)

#
# Also prehistoric sequential calculation.
# However, it works for segments.
#
def mul_list(RNG):
	if len(RNG) == 1:
		return RNG[0]
	else:
		fact = 1
		for i in range(len(RNG)):
			fact *= RNG[i]
		return fact

#
# A very mathematical approach
# Prime factorization algorithm
#

# 
# Multiplicity: Returns the power of a prime number p in the N!
def multiplicity(N, p):
	if p > N:
		return 0
	if p > N/2:
		return 1
	q, m = N, 0
	while q >= p:
		q /= p
		m += q
	return m

#
# Compute the explicit of a factored integer.
# read: exponentation by squaring
def powproduct(ns):
	
	result = 1
	base, exp = ns

	if ns == 0 or exp == 0:
		return result

	while exp != 0:
		if exp == 1:
			result *= base
			return result
		elif exp%2 != 0:
			result *= base
			exp -= 1
		else:
			base = (base**2)
			exp /= 2

#
# Generate a list of prime numbers within n!
def primes(N):
	sieve = list(range(N))
	sieve[:2] = [0, 0]
	for i in range(2, int(N**0.5)+1):
		if sieve[i] != 0:
			for j in range(i**2, N, i):
				sieve[j] = 0

	return [p for p in sieve if p]

#
# prime factorization method
def primefact(N):
	prime_num_list = primes(N)
	prime_table = []
	factN = 1

	for p_num in prime_num_list:
		prime_table.append((p_num, multiplicity(N, p_num)))

	for base_exp_tuple in prime_table:
		factN *= powproduct(base_exp_tuple)

	return factN

#
# Splits a list into designated chunks of even size.
#
def split_list(list_N, n_of_chunks):
	chunk_length = int(len(list_N)/n_of_chunks)
	list_remains = len(list_N)%n_of_chunks
	if list_remains == 0:
		return [list_N[i:i+chunk_length] \
			for i in range(0, len(list_N), chunk_length)]
	else:
		list_N_seg = []
		list_N_adj_remain = list_N[:(chunk_length+1)*list_remains]
		list_N_div = list_N[(chunk_length+1)*list_remains:len(list_N)]

		list_N_seg = [list_N_adj_remain[i:i+(chunk_length+1)] \
			for i in range(0, len(list_N_adj_remain), chunk_length+1)]\
			+\
			[list_N_div[i:i+chunk_length] \
			for i in range(0, len(list_N_div), chunk_length)]

		return list_N_seg

#
# Main DNC algorithm. Receives N as factorial number.
#
def dnc(N, chunks_dnc=5000):
	if chunks_dnc >= N:
		print(("Too many divisions requested!!: %d >= %d"%(chunks_dnc, N)))
		print ("Assuming single segment chunks.")
		return seq_fact(N), 1
	else:
		# Generating Parts
		N_list = list(range(1, N+1, 1))
		N_seg = split_list(N_list, chunks_dnc)
		N_list = []

		fact_result = 1
		for i in range(len(N_seg)):
			fact_result *= mul_list(N_seg[i])
			N_seg[i] = []

		#print fact_result
		return fact_result, chunks_dnc

# 
# dnc with segment
#
def dnc_seg(N_seg_list, chunks_dnc_seg=100):
	if chunks_dnc_seg >= len(N_seg_list):
		print(("Too many divisions for a segment requested!!: %d >= %d" % \
			(chunks_dnc_seg, N_seg_list)))
		chunks_dnc_seg = N
		return mul_list(N), chunks_dnc_seg
	else:
		N_dnc_seg = split_list(N_seg_list, chunks_dnc_seg)
		fact_result = 1
		for i in range(len(N_dnc_seg)):
			fact_result *= mul_list(N_dnc_seg[i])
			N_dnc_seg[i] = []
		return fact_result, chunks_dnc_seg

#
# DNC with multiprocessing
#
#
# Worker function for dnc_m
# Second input is actually output which was defined by 
# Queue() datatype.
def dnc_m_worker(N, fact_result_q):
	fact_result_q.put(mul_list(N))

#
# Multiprocessing Divide and Conquer (I believe this is the 'real' DNC)
#
def dnc_m(N, processes_dnc=mp.cpu_count()):
	if processes_dnc >= N:
		print(("Too many divisions requested!!: %d >= %d"%(processes_dnc, N)))
		print ("Assuming single segment processes.")
		return seq_fact(N), 1
	else:
		N_list = list(range(1, N+1, 1))
		N_seg = split_list(N_list, processes_dnc)
		N_list = []
		#print N_seg
		
		fact_result = 1
		fact_result_q = mp.Queue()
		procs = []
		for i in range(len(N_seg)):
			p = mp.Process(target=dnc_m_worker, args=(N_seg[i], fact_result_q))
			procs.append(p)
			p.start()
		
		#i = 0
		for p in procs:	
			#print ['***** Iteration', i, ' *****']
			#print ['Segment for ', procs[i], ' is ', N_seg[i] ]
			#print ['Segment Factorial: ', fact_result_q.get()]
			fact_result *= fact_result_q.get()
			#print ['result: ', fact_result]
			#print [procs[i], 'Factorial value', fact_result]
			#i += 1

		for p in procs:
			p.join()

		#print fact_result
		return fact_result, processes_dnc

#
# DNC with multiprocessing of recursive DNC
#
# Worker function for dnc_ml
# Second input is actually output which was defined by 
# Queue() datatype.
def dnc_ml_worker(N, fact_result_q):
	if len(N) < 1000:
		segments = 100
	elif len(N) >= 1000 and len(N) < 10000:
		segments = 500
	elif len(N) >= 10000 and len(N) < 50000:
		segments = 1000
	else:
		segments = 5000
	factN_dnc, chunks_dnc_child = dnc_seg(N, segments)
	fact_result_q.put(factN_dnc)

#
# Multiprocessing Divide and Conquer (I believe this is the 'real' DNC)
#
def dnc_ml(N, processes_dnc=mp.cpu_count()):
	if processes_dnc > N:
		print(("Too many divisions requested!!: %d >= %d"%(processes_dnc, N)))
		print ("Assuming single segment processes.")
		return seq_fact(N), 1
	else:
		N_list = list(range(1, N+1, 1))
		N_seg = split_list(N_list, processes_dnc)
		N_list = []
		
		fact_result = 1
		fact_result_q = mp.Queue()
		procs = []
		for i in range(len(N_seg)):
			p = mp.Process(target=dnc_ml_worker, args=(N_seg[i], fact_result_q))
			procs.append(p)
			p.start()
		
		#i = 0
		for p in procs:	
			#print ['***** Iteration', i, ' *****']
			#print ['Segment for ', procs[i], ' is ', N_seg[i] ]
			#print ['Segment Factorial: ', fact_result_q.get()]
			fact_result *= fact_result_q.get()
			#print ['result: ', fact_result]
			#print [procs[i], 'Factorial value', fact_result]
			#i += 1

		for p in procs:
			p.join()

		#print fact_result
		return fact_result, processes_dnc

#
# Choosing algorithm adaptively.
# 
# It seems some algorithm has advantage on others on different number 
# ranges.
def factN_adaptive(N):
	if N <= 0: 
		print ("Factorial calculation requires positive number!!")
		exit(1)
	elif 0 <= N and N <= 12:
		return fact_trivial(N), 1
	elif 12 < N and N <= 20 and utils.is32bit() == False:
		return fact_trivial64(N), 1
	elif 12 < N and N <= 20:
		#print ("Using recursive algorithm")
		return rseq_fact(N), 1
	elif 20 < N and N <= 100:
		return seq_fact(N), 1
	elif 100 < N and N <= 1000:
		#print ("Using sequential algorithm")
		return primefact(N), 1
	elif 1000 < N and N <= 10000:
		#print ("Using sequential DNC algorithm")
		return dnc(N, 500)
	elif 10000 < N and N <= 100000:
		#print ("Using multiprocessing DNC algorithm")
		return dnc_m(N)
	else:
		#print ("Using multiprocessing DNC algorithm")
		return dnc_ml(N)

