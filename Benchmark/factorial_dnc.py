#!/usr/bin/env python
#
# Calculates factorial and displays/saves elipsed time in seconds.
# Utilized 'dnc' algorithm... I believe.
#

import time
import factor_lib as flib
import utils

#
# factN_dnc
#
# Calculates Sequencial Factorial and displays time.
# Also saves the file if filename has been defined as string.
# Uses Divide and Conquer algorithm... I think.
#
def factN_dnc(N, savefile=None, chunks=5000):
	print ("Calculating Divide and Conquer Algorithm...")
	save_table = [None] * len(N)
	factN = []
	factNdnc = []

	for i in range(len(N)):
		startTime = time.clock()
		factN, chunks = flib.dnc(N[i],chunks)
		endTime = time.clock()
		time_diff = round(endTime-startTime,6)
		print ("%d! with %d chunks has been finished in %.6f seconds."%\
			(N[i], chunks, time_diff))
		save_table[i] = str(N[i])+'\t'+str(chunks)+'\t'+str(time_diff)

	if savefile == None:
		return time_diff
	else:
		return time_diff, utils.savelist(savefile, save_table)

#
# factN_dnc_m
#
# Same DNC algorithm with multiprocessing feature.
#
def factN_dnc_m(N, savefile=None, processes=5000):
	print ("Calculating Divide and Conquer Algorithm with Multiprocess model...")
	save_table = [None] * len(N)
	factN = []
	factNdnc = []

	for i in range(len(N)):
		startTime = time.clock()
		factN, chunks = flib.dnc_m(N[i], processes)
		endTime = time.clock()
		time_diff = round(endTime-startTime,6)
		print ("%d! with %d processes has been finished in %.6f seconds."%\
			(N[i], chunks, time_diff))
		save_table[i] = str(N[i])+'\t'+str(processes)+'\t'+str(time_diff)

	if savefile == None:
		return time_diff
	else:
		return time_diff, utils.savelist(savefile, save_table)
