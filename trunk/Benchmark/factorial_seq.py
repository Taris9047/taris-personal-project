#!/usr/bin/env python
#
# Calculates factorial and displays/saves elipsed time in seconds.
#

import time
import factor_lib as flib
import utils

#
# factN_seq
#
# Calculates Sequencial Factorial and displays time.
# Also saves the file if filename has been defined as string.
#
def factN_seq(N, savefile=None):
	print ("Calculating Generic Sequential Algorithm...")
	save_table = [None] * len(N)
	factN = []
	factNdnc = []

	for i in range(len(N)):
		startTime = time.clock()
		factN = flib.factorial(N[i])
		endTime = time.clock()
		time_diff = round(endTime-startTime, 6)
		print ("%d!	has been finished in %.6f seconds."%\
			(N[i], time_diff))
		save_table[i] = str(N[i])+'\t'+'1'+'\t'+str(time_diff)

	if savefile == None:
		return time_diff
	else:
		return time_diff, utils.savelist(savefile, save_table)