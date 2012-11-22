# Factorial (normal) library
#
import multiprocessing as mp

#
# Sequential algorithm. 
# Very prehistoric but working.
#
def factorial(N):
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
def rfactorial(N):
	if N == 1:
		return 1
	else:
		return N * rfactorial (N-1)

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
# Main DNC algorithm. Receives N as factorial number.
#
def dnc(N, chunks):
	if chunks > N:
		print ("Too many divisions requested!!: %d > %d"%(chunks, N))
		print ("Assuming single segment chunks.")
		chunks = N

	# Generating Parts
	part_len = N/chunks
	N_list = range(1, N+1, 1)
	N_seg = [N_list[i:i+part_len] for i in range(0, N, part_len)]
	N_list = []

	fact_result = 1
	for i in range(len(N_seg)):
		fact_result *= mul_list(N_seg[i])
		N_seg[i] = []

	#print fact_result
	return fact_result, chunks

# Worker function for dnc_m
# Second input is actually output which was defined by 
# Queue() datatype.
def dnc_m_worker(N, fact_result_q):
		fact_result_q.put(mul_list(N))

#
# Multiprocessing Divide and Conquer (I believe this is the 'real' DNC)
#
def dnc_m(N, chunks):
	if chunks > N:
		print ("Too many divisions requested!!: %d > %d"%(chunks, N))
		print ("Assuming single segment chunks.")
		chunks = N
	
	part_len = N/chunks
	N_list = range(1, N+1, 1)
	N_seg = [N_list[i:i+part_len] for i in range(0, N, part_len)]
	N_list = []
	print N_seg
	
	fact_result = 1
	fact_result_q = mp.Queue()
	procs = []
	for i in range(len(N_seg)):
		p = mp.Process(target=dnc_m_worker, args=(N_seg[i], fact_result_q))
		procs.append(p)
		p.start()
	
	i = 0
	for p in procs:	
		print ['***** Iteration', i, ' *****']
		print ['Segment for ', procs[i], ' is ', N_seg[i] ]
		print ['Segment Factorial: ', fact_result_q.get()]
		fact_result *= fact_result_q.get()
		print ['result: ', fact_result]
		#print [procs[i], 'Factorial value', fact_result]
		i += 1

	for p in procs:
		p.join()

	print fact_result
	return fact_result, chunks
