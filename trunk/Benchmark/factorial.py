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
		factN = flib.seq_fact(N[i])
		endTime = time.clock()
		time_diff = round(endTime-startTime, 6)
		print ("%d!	has been finished in %.6f seconds."%\
			(N[i], time_diff))
		save_table[i] = str(N[i])+'\t'+'1'+'\t'+str(time_diff)

	if savefile == None:
		return factN
	else:
		return factN, utils.savelist(savefile, save_table)


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
		factN, chunks_result = flib.dnc(N[i],chunks)
		endTime = time.clock()
		time_diff = round(endTime-startTime,6)
		print ("%d! with %d chunks has been finished in %.6f seconds."%\
			(N[i], chunks_result, time_diff))
		save_table[i] = str(N[i])+'\t'+str(chunks)+'\t'+str(time_diff)

	if savefile == None:
		return factN
	else:
		return factN, utils.savelist(savefile, save_table)

#
# factN_dnc_m
#
# Same DNC algorithm with multiprocessing feature.
#
import multiprocessing as mp
def factN_dnc_m(N, savefile=None, processes=mp.cpu_count()*2):
	print ("Calculating Divide and Conquer Algorithm with Multiprocess model...")
	save_table = [None] * len(N)
	factN = []
	factNdnc = []

	for i in range(len(N)):
		startTime = time.clock()
		factN, processes_result = flib.dnc_m(N[i], processes)
		endTime = time.clock()
		time_diff = round(endTime-startTime,6)
		print ("%d! with %d processes has been finished in %.6f seconds."%\
			(N[i], processes_result, time_diff))
		save_table[i] = str(N[i])+'\t'+str(processes)+'\t'+str(time_diff)

	if savefile == None:
		return factN
	else:
		return factN, utils.savelist(savefile, save_table)

#
# factNa
#
# Calculates Sequencial Factorial and displays time.
# Also saves the file if filename has been defined as string.
# Uses adoptive algorithm... 
#
def factNa(N, savefile=None):
	print ("Calculating with Various Algorithms...")
	save_table = [None] * len(N)
	factN = []
	factNdnc = []

	for i in range(len(N)):
		startTime = time.clock()
		factN, processes_result = flib.factN_adaptive(N[i])
		endTime = time.clock()
		time_diff = round(endTime-startTime,6)
		print ("%d! with %d chunk(process)s has been finished in %.6f seconds."%\
			(N[i], processes_result, time_diff))
		save_table[i] = str(N[i])+'\t'+str(processes_result)+'\t'+str(time_diff)

	if savefile == None:
		return factN
	else:
		return factN, utils.savelist(savefile, save_table)
