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

import factorial_seq as factNs
import factorial_dnc as factNd
import factorial_adaptive as factNa
import utils

# detecting IronPython. For now, it will
# just prevent running multiprocessing module
# when IronPython is detected.
import sys
if '.NET' in sys.version:
	print ('Detected IronPython...')
	__IronPython__ = True
else:
	__IronPython__ = False

def main():
	if __IronPython__ == False:
		import multiprocessing as mp
		mp.freeze_support()

	ver_number = '0.0.4.2'
	print ("*** T-Bench ver. Abysmal "+ver_number+" ***")
	print (" ")
	print ("    T-Bench Copyright (C) 2012  Taylor Shin.\n\
    This program comes with ABSOLUTELY NO WARRANTY.\n\
    This is free software, and you are welcome to redistribute it.\n")

	# Running routines
	#N = [1000, 5000, 10000, 50000, 100000]
	N = [10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000]
	#N = [1000, 5000, 10000]
	#N = [10030]
	print (utils.sysinfo())

	N_str = ''
	for n in N:
		N_str += str(n)+', '
	print ("Calculation range: "+N_str)

	factNs.factN_seq(N, "Sequential.txt")
	print (" ")
	factNd.factN_dnc(N, "DNC.txt", 500)
	print (" ")
	# Taking Advantage of Multiprocessing module.
	if __IronPython__ == False:
		factNd.factN_dnc_m(N, "DNC_m.txt", 16)
		print(" ")

	factNa.factNa(N, "Adaptive.txt")
	print(" ")

	return 0

if __name__ == '__main__':
	main()
