# Factorial lib with GMPY2 addition
#
import gmpy2 as gmp
import multiprocessing as mp

#
# Direct return
# In case of very small number... why calculate?
def fact_trivial(N):
	factN = [1, 1, 2, 6, 24, 120, 720, 5040, 
		40320, 362880, 3628800, 39916800, 479001600]
	if N < 0:
		print "Seriousely, just multiply -1!!"
		raise ValueError("%d is negative!!"%N)
	elif N > 12:
		print "Oops, too large for this function!!"
		raise ValueError("%d is too large!!"%N)
	else:
		return gmp.mpz(factN[N])


#
# Sequential algorithm. 
# Very prehistoric but working.
#
def seq_fact(N):
	sys.setrecursionlimit(n*1000)
	fact = gmp.mpz(1)

	for i in range(1,N+1,1):
		if N == 1:
			return 1
		else:
			fact *= gmp.mpz(i)

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
		return N * gmp.mpz(rseq_fact (N-1))

#
# Also prehistoric sequential calculation.
# However, it works for segments.
#
def mul_list(RNG):
	if len(RNG) == 1:
		return RNG[0]
	else:
		fact = gmp.mpz(1)
		for i in range(len(RNG)):
			fact *= gmp.mpz(RNG[i])
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
	if p > N//2:
		return 1
	q, m = N, 0
	while q >= p:
		q //= p
		m += q

	return m

#
# Generate a list of prime numbers within n!
def primes(n):
	sieve = range(0,n+1,1)
	sieve[:2] = [0, 0]
	for i in range(2, int(n**0.5)+1,1):
		if sieve[i] != 0:
			for j in range(i**2, n, i):
				sieve[j] = 0

	return [p for p in sieve if p]

#
# Compute the explicit of a factored integer.
# read: exponentation by squaring
def powproduct(ns):
	if ns == 0:
		return 1

	units = 1
	multi = []

	for base, exp in ns:
		if exp == 0:
			continue
		elif exp == 1:
			units *= base
		else:
			if (exp // 2) != 0:
				units *= base
			multi.append((base, exp//2))
	return units * powproduct(multi)**2

#
# prime factorization method
def primefact(n):
	n = gmp.mpz(n)
	sys.setrecursionlimit(n*1000)
	return powproduct((gmp.mpz(p), multiplicity(n,p)) for p in primes(n))



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
		print ("Too many divisions requested!!: %d >= %d"%(chunks_dnc, N))
		print ("Assuming single segment chunks.")
		return seq_fact(N), 1
	else:
		# Generating Parts
		N_list = range(1, N+1, 1)
		N_seg = split_list(N_list, chunks_dnc)
		N_list = []

		fact_result = gmp.mpz(1)
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
		print ("Too many divisions for a segment requested!!: %d >= %d" % \
			(chunks_dnc_seg, N_seg_list))
		chunks_dnc_seg = N
		return mul_list(N), chunks_dnc_seg
	else:
		N_dnc_seg = split_list(N_seg_list, chunks_dnc_seg)
		fact_result = gmp.mpz(1)
		for i in range(len(N_dnc_seg)):
			fact_result *= mul_list(N_dnc_seg[i])
			N_dnc_seg[i] = []
		return fact_result, chunks_dnc_seg

#
# DNC with multiprocessing
#
#
# Multiprocessing doesn't work with IronPython.
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
		print ("Too many divisions requested!!: %d >= %d"%(processes_dnc, N))
		print ("Assuming single segment processes.")
		return seq_fact(N), 1
	else:
		N_list = range(1, N+1, 1)
		N_seg = split_list(N_list, processes_dnc)
		N_list = []
		#print N_seg
		
		fact_result = gmp.mpz(1)
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
		print ("Too many divisions requested!!: %d >= %d"%(processes_dnc, N))
		print ("Assuming single segment processes.")
		return seq_fact(N), 1
	else:
		N_list = range(1, N+1, 1)
		N_seg = split_list(N_list, processes_dnc)
		N_list = []
		
		fact_result = gmp.mpz(1)
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
def factN_adaptive(n):
	if n <= 0: 
		print ("Factorial calculation requires positive number!!")
		exit(1)
	elif 0 <= n and n <= 12:
		return fact_trivial(N)
	elif  12 < n and n <= 100:
		#print ("Using recursive algorithm")
		return rseq_fact(n), 1
	elif 100 < n and n <= 1000:
		#print ("Using sequential algorithm")
		return seq_fact(n), 1
	elif 1000 < n and n <= 10000:
		#print ("Using sequential DNC algorithm")
		return dnc(n, 500)
	elif 10000 < n and n <= 100000:
		if __IronPython__ == True:
			#print ("Using sequential DNC algorithm")
			return dnc(n, 5000)
		else:
			#print ("Using multiprocessing DNC algorithm")
			return dnc_m(n)
	else:
		# Todo: change here with better algorithm
		# if implemented...
		#print ("Using multiprocessing DNC algorithm")
		return dnc_ml(n)


