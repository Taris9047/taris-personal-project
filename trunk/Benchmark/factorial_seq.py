#!/usr/bin/env python
#
# Calculates factorial and displays/saves elipsed time in seconds.
#

import time
import factor_lib as flib
import utils
import multiprocessing as mp


#
# factN_seq
#
# Calculates Sequencial Factorial and displays time.
# Also saves the file if filename has been defined as string.
#
def factN_seq(N, savefile=None):
	print ("Calculating Generic Sequential Algorithm...")
	time_diff = [None] * len(N)
	factN = []
	factNdnc = []

	for i in range(len(N)):
		startTime = time.clock()
		factN = flib.factorial(N[i])
		endTime = time.clock()
		time_diff[i] = round(endTime-startTime, 6)
		print ("%d!	has been finished in %.6f seconds."%(N[i], time_diff[i]))

	list_to_save = utils.arrange_data(N, time_diff)

	if savefile == None:
		return time_diff
	else:
		return time_diff, utils.savelist(savefile, list_to_save)

#
# factN_seq_m
#
# Same sequential algorithm with multiprocessing feature.
#
def factN_seq_m(N, savefile=None):
	print "With multiprocessing module..."
	p = mp.Process(target=factN_seq, args=(N,savefile))
	p.start()
	p.join()