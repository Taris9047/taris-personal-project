#!/usr/bin/env python

"""

Dummy test file generator script

Written by Taylor Shin

March 14th 2017

"""

import sys
import string
import random

def rand_char_gen():
	str_set = \
		string.ascii_lowercase + \
		string.ascii_uppercase + \
		string.digits

	ind = random.randrange(len(str_set))

	return str_set[ind];


def main():
	data_size = 1000;
	if len(sys.argv) == 2:
		data_size = int(sys.argv[1])

	print("Setting up data size...", data_size)

	multiplier = 1000;

	dataf_name = 'data.txt'

	with open(dataf_name, 'w') as fp:
		data_str = ''
		for i in range(data_size*multiplier):
			data_str += rand_char_gen()

		data_str += '\n'
		fp.write(data_str)

	print("Written ", data_size*multiplier, " characters.")

if __name__ == "__main__":
	main();
