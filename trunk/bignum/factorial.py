#!/usr/bin/env python

import sys
from PBN import Bignum
import math as m
import time

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


if len(sys.argv) >= 2:
	input_num = sys.argv[1]
else:
	input_num = 10

start_time = time.clock()
fact_num = factorial(int(input_num))
print "%s! = %s\n" % (input_num, fact_num.N())
end_time = time.clock()
PBN_time = end_time - start_time
print "%s! was finished in %f seconds with PBN\n" % (input_num, PBN_time)

del fact_num

start_time = time.clock()
fact_num = m.factorial(int(input_num))
print "%s! = %s\n" % (input_num, fact_num)
end_time = time.clock()
builtin_time = end_time - start_time
print "%s! was finished in %f seconds with built-in math\n" % (input_num, builtin_time)

print "PBN was %f times slower than built-in for %s!\n" % (PBN_time/builtin_time, input_num)



