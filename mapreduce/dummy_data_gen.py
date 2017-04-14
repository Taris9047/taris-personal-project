#!/usr/bin/env python

"""

Dummy test file generator script

Written by Taylor Shin

March 14th 2017

"""

import sys
import string
import random
import datetime as dt

rows = 8 # bump it to 32 later...
img_res_x = rows*6
img_res_y = rows*6
grey_scale_max = 2**24

def rand_char_gen():
	str_set = \
		string.ascii_lowercase + \
		string.ascii_uppercase + \
		string.digits

	ind = random.randrange(len(str_set))

	return str_set[ind];

def timestamp_gen():
	return dt.datetime.now().strftime("%f")

"""@img_gen

	Make a random image data

"""
def img_gen():
	img_data = {}
	img_data["timestamp"] = timestamp_gen()
	img_data["res_info"] = (img_res_x, img_res_y)
	img_data["image"] = \
		[[random.randrange(grey_scale_max) for _ in range(img_res_y)] \
			for _ in range(img_res_x)]
	img_data["label"] = ''

	label_len = random.randrange(15)+1
	for i in range(label_len):
		img_data["label"] += rand_char_gen()

	return img_data

"""@get_unique_rand_num

"""
def get_unique_rand_num(max_rng, n_avoid):
	while (True):
		ret_num = random.randrange(max_rng)
		if ret_num != n_avoid:
			break
	return ret_num

"""@shuffle_data

	Shuffle list of data stirngs

"""
def shuffle_data(data, n_shfl=None):
	if not isinstance(data, list):
		data = [data]

	data_len = len(data)

	if data_len < 2:
		return data

	if not n_shfl:
		n_shfl = int(data_len/2)

	for n in range(n_shfl):
		for i in range(data_len):
			ti = get_unique_rand_num(data_len, i)

			tmp = data[i];
			data[i] = data[ti]
			data[ti] = tmp

	return data

"""@rand_data_gen

	Let's make list of strings from imaginary imgs...

	Mimmicing bunch of data packets from ROIC

"""
def rand_data_gen():
	data_str_set = []
	img = img_gen()

	ts = img["timestamp"]
	x_len, y_len = img["res_info"]
	label = img["label"]

	packet = ''
	for i in range(x_len):
		for j in range(y_len):
			gs = img["image"][i][j]
			packet = '%s %s %d %d %d' % \
				(ts, label, i, j, gs)
			data_str_set.append(packet)

	return shuffle_data(data_str_set)

"""@

	The main function

"""
def main():
	data_size = 10;
	if len(sys.argv) == 2:
		data_size = int(sys.argv[1])

	print("Setting up %d images ... " % data_size)

	# Let's generate thousands of data stream...
	data_set = []
	for i in range(data_size):
		an_img_data_set = rand_data_gen()
		data_set += an_img_data_set
		print("Data set generated %d/%d" % (i+1, data_size))
	data_set = shuffle_data(data_set)

	dataf_name = 'data.txt'
	with open(dataf_name, 'w') as fp:
		data_str = ''
		for i in range(len(data_set)):
			data_str += data_set[i]
			data_str += '\n'
		fp.write(data_str)

	print("Written", data_size, "random pseudo image data.")

if __name__ == "__main__":
	main();
