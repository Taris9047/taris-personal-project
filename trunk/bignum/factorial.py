#!/usr/bin/env python
import sys
from PBN import Bignum
import math
import time
import subprocess as sp

def factorial(num):
	bignum = Bignum(1)
	index = Bignum(1)

	if num == 0:
		return 1
	else:
		for i in range(num):
			bignum = bignum*index
			index = index + Bignum(1)
		
	return bignum

def factorial_statistics(num, trials):
	time_array = [0]*trials
	
	for i in range(trials):
		start_time = time.clock()
		factorial(int(num))
		end_time = time.clock()
		time_array[i] = end_time - start_time

	return time_array

def mfactorial_statistics(num, trials):
	time_array = [0]*trials
	
	for i in range(trials):
		start_time = time.clock()
		math.factorial(int(num))
		end_time = time.clock()
		time_array[i] = end_time - start_time

	return time_array
	

def main():
	if len(sys.argv) >= 2:
		input_num = int(sys.argv[1])
	else:
		input_num = 10

	PBY_avg = [0]*input_num
	builtin_avg = [0]*input_num
	for i in range(input_num):
		print "Running estimation for %d!" % (i+1)
		PBY_result = factorial_statistics(i+1, 100)
		builtin_result = mfactorial_statistics(i+1, 100)
		PBY_avg[i] = reduce(lambda x, y: x + y, PBY_result)/len(PBY_result)
		builtin_avg[i] = reduce(lambda x, y: x + y, builtin_result)/len(builtin_result)

	print ('Writing result up to %d! to file\n' % (input_num))
	file = open('factorial_benchmark.txt', 'w')
	file.write('Fact_Num\tPBY\tBuilt-in\n')
	for i in range(input_num):
		line_text = str(i+1)+'\t'+"%e"%(PBY_avg[i])+'\t'+"%e"%(builtin_avg[i])+'\t'+'\n'
		file.write(line_text)

	file.close()

	'''
	print "Comparison:"
	print "%d! in PBY vs. %d! in built-in\n" % (int(input_num), int(input_num))
	print "PBY average: %f, built-in average: %f" % (PBY_avg, builtin_avg)
	print "PBY is %f slower than built-in math\n" % (PBY_avg/builtin_avg)
	'''
	sp.call(["gnuplot", "plot.gp"])
	
if __name__ == "__main__":
	main()
