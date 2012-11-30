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

ver_number = "0.0.4.4"

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
	#N_default = [1000, 5000, 10000, 50000, 100000]
	N_default = [10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000]
	#N_default = [1000, 5000, 10000]
	#N_default = [100300]

	print (utils.sysinfo())

	# setting up N
	if run_options[1] == []:
		N = N_default
	else:
		N = run_options[1]

	N_str = ''
	for n in N:
		N_str += str(n)+', '
	print ("Calculation range: "+N_str)
	
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
				random.seed(time.clock())
				N = [random.randrange(1000000,100000000)]
				gfactN.factNa(N)
		else:
			print ("Wrong Option!! You just found the Program Bug!!")


	return 0

if __name__ == '__main__':
	main()
