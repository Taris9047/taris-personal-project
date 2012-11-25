#!/usr/bin/env python

#
# Calculates factorial and displays/saves elipsed time in seconds.
# Utilized 'dnc' algorithm... I believe.
#

import time
import factor_lib as flib
import utils

# detecting IronPython. For now, it will
# just prevent running multiprocessing module
# when IronPython is detected.
import sys
if '.NET' in sys.version:
	__IronPython__ = True
else:
	__IronPython__ = False

#
# factN
#
# Calculates Sequencial Factorial and displays time.
# Also saves the file if filename has been defined as string.
# Uses adoptive algorithm... 
#
def factNa(N, savefile=None):
	print ("Calculating with Various Algorithm...")
	save_table = [None] * len(N)
	factN = []
	factNdnc = []

	for i in range(len(N)):
		startTime = time.clock()
		factN = flib.factN_adaptive(N[i])
		endTime = time.clock()
		time_diff = round(endTime-startTime,6)
		print ("%d! has been finished in %.6f seconds."%\
			(N[i], time_diff))
		save_table[i] = str(N[i])+'\t'+str(time_diff)

	if savefile == None:
		return time_diff
	else:
		return time_diff, utils.savelist(savefile, save_table)

