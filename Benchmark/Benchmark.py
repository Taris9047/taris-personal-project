#!/usr/bin/env python

# T-Bench a simple factorial calculation benchmark script(or program.)
# Copyright (C) 2012  Taylor Shin

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


#
# Factorial benchmark master file
# requires GMPY2 package to run gmp routines
#

import factorial as factN
import gfactorial as gfactN
import random
import time
import utils
import sys

ver_number = "0.0.4.5"

def main():
	import multiprocessing as mp
	mp.freeze_support()

	print ("*** T-Bench ver. Abysmal "+ver_number+" ***")
	print (" ")
	print ("    T-Bench Copyright (C) 2012  Taylor Shin.\n\
    This program comes with ABSOLUTELY NO WARRANTY.\n\
    This is free software, and you are welcome to redistribute it.\n")

	run_options = utils.bench_options(sys.argv)
	print ("Options set as: ")
	print run_options

	# Running routines
	# setting up N
	N_default = [10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000]
	if run_options[1] == []:
		N = N_default
	else:
		N = run_options[1]

	utils.sysinfo(True)
	utils.print_range(N)

	for bopts in run_options[0]:
		if bopts == 'seq':
			factN.factN_seq(N, "nSequential.txt")
			print (" ")
		elif bopts == 'dnc':
			factN.factN_dnc(N, "nDNC.txt", 500)
			print (" ")
		elif bopts == 'dnc_m':
			# Taking Advantage of Multiprocessing module.
			factN.factN_dnc_m(N, "nDNC_m.txt")
			print(" ")
		elif bopts == 'auto':
			factN.factNa(N, "nAdaptive.txt")
			print(" ")
		elif bopts == 'gseq':
			gfactN.factN_seq(N, "gmpSequential.txt")
			print (" ")
		elif bopts == 'gdnc':
			gfactN.factN_dnc(N, "gmpDNC.txt", 500)
			print (" ")
		elif bopts == 'gdnc_m':
			# Taking Advantage of Multiprocessing module.
			gfactN.factN_dnc_m(N, "gmpDNC_m.txt")
			print(" ")
		elif bopts == 'gauto':
			gfactN.factNa(N, "gmpAdaptive.txt")
			print(" ")
		elif bopts == 'torture':
			while 1:
				print (" ")
				print ("*** Torture Test Mode ***")
				print ("  Hit Ctrl+C to break out...")
				print ("  Ignoring defined N list.")
				random.seed()
				rand_min = 100
				rand_max = 10000000
				N = [random.randrange(rand_min,rand_max)]
				print ("Calculating %d!"%N[0])
				print ("Without GMP")
				factN_nogmp = factN.factNa(N)
				print ("With GMP")
				factN_gmp = gfactN.factNa(N)
				
				if factN_nogmp == factN_gmp:
					print("Numbers match!! next...")
				else:
					print("Uh oh... something is wrong... ")
					print("No gmp value: %d does not match with gmp value: %d !!"\
						%(factN_nogmp, factN_gmp))
					exit(1)

				print (" ")
		else:
			print ("Wrong Option!! You just found the Program Bug!!")


	return 0

if __name__ == '__main__':
	main()
