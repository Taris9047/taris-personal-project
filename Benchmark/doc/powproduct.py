#!/usr/bin/env python
def powproduct(ns):	
	
	result = 1
	base, exp = ns

	if ns == 0 or exp == 0:
		return result

	#loop = 0
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

		#print ''
		#print 'loop #: ', loop
		#print 'base is', base
		#print 'exp is', exp
		#print 'result is', result

		#loop += 1


def multiplicity(n, p):
    """Return the power of the prime number p in the
    factorization of n!"""
    if p > n: return 0
    if p > n//2: return 1
    q, m = n, 0
    while q >= p:
        q //= p
        m += q
    return m

def primes(N):
	sieve = list(range(N))
	sieve[:2] = [0, 0]
	for i in range(2, int(N**0.5)+1):
		if sieve[i] != 0:
			for j in range(i**2, N, i):
				sieve[j] = 0

	return [p for p in sieve if p]


if __name__ == '__main__':
	base = 2
	power = 3
	result = powproduct((base,power))
	print str(base)+'^'+str(power)+' is: '+str(result)

	print primes(10)
	#print multiplicity(100,11)
