#!/usr/bin/env python
#
# Factorial benchmark master file
# requires GMPY2 package to run gmp routines
#

import factorial_seq as factNs
import factorial_dnc as factNd
import utils

def main():
	print ("*** T-Bench ver. Abysmal 0001 ***")
	print (" ")

	# Running routines
	N = [1000, 5000, 10000, 50000, 100000]
	#N = [1000, 5000, 10000]
	print (utils.sysinfo())
	print ("Calculation range: ",N)

	factNs.factN_seq(N, "Sequential.txt")
	print (" ")
	factNd.factN_dnc(N, "DNC.txt")
	print (" ")

	# Taking Advantage of Multiprocessing module
	#factNd.factN_dnc_m(N, "DNC_m.txt")
	#print(" ")
	# TODO: Needs a real multiprocess routine.

	return 0

if __name__ == '__main__':
	main()
