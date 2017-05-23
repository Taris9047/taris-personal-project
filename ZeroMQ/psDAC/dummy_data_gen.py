#!/usr/bin/env python3

"""

Dummy test file generator script

Written by Taylor Shin

March 14th 2017

"""

import os
import sys
import string
import random
import datetime as dt
import tarfile

# MiniVIPIC: 32 x 32 per chip, 6 x 6 chips
# VIPIC-L: 192 x 192 per chip, 6 x 6 chips
rows = 32
img_res_x = rows*6
img_res_y = rows*6
gs_bits = 24
grey_scale_max = 2**gs_bits

"""@str_to_barray

	string to binary array

"""
def str_to_barray(some_str):
	return bytearray([ord(l) for l in some_str])


"""@rand_char_gen

	Generates random characters

"""
def rand_char_gen():
	str_set = \
		string.ascii_lowercase + \
		string.ascii_uppercase + \
		string.digits

	ind = random.randrange(len(str_set))

	return str_set[ind];


"""@timestamp_gen

	Generates random timestamp

"""
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

	label_len = random.randrange(255)
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

	--> Ha, random.shuffle is WAAAAY faster than my crappy code!!

"""
def shuffle_data(data):
	random.shuffle(data)
	return data


"""@rand_data_gen

	Let's make list of strings from imaginary imgs...

	Mimmicing bunch of data packets from ROIC

"""
def rand_data_gen():
	data_set = []
	img = img_gen()

	ts = img["timestamp"]
	x_len, y_len = img["res_info"]
	label = img["label"]

	data_bin_set = []
	for i in range(x_len):
		for j in range(y_len):
			gs = img["image"][i][j]
			packet = \
				len(ts).to_bytes(1, sys.byteorder) + \
				str_to_barray(ts) + \
				len(label).to_bytes(1, sys.byteorder) + \
				str_to_barray(label) + \
				i.to_bytes(1, sys.byteorder) + \
				j.to_bytes(1, sys.byteorder) + \
				gs.to_bytes(int(gs_bits/8), sys.byteorder) + \
				str_to_barray('\n')
			data_bin_set.append(bytearray(packet))

	return data_bin_set

"""@main

	The main function

"""
def main():
	n_images = 10;
	dataf_name = './data.dat'
	if len(sys.argv) >= 2:
		n_images = int(sys.argv[1])
	if len(sys.argv) >= 3:
		n_images = int(sys.argv[1])
		dataf_name = sys.argv[2]

	print("Setting up %d images ... " % n_images)

	# Let's generate thousands of data stream...
	data_set = []
	for i in range(n_images):
		an_img_data_set = rand_data_gen()
		data_set += an_img_data_set
		txt = "Data set generated %d/%d [%.2f %%]\r" % (i+1, n_images, 100*(i+1)/n_images)
		sys.stdout.write(txt)
		sys.stdout.flush()
	print("")
	print("Shuffling %d image(s)..." % (n_images))
	data_set = shuffle_data(data_set)

	data_set_len = len(data_set)

	with open(dataf_name, 'wb') as fp:
		print("Writing %d entries to... %s" % (data_set_len, dataf_name))
		data_to_write = bytearray()
		data_to_write += str_to_barray('psDAC_Input\n')
		for i in range(data_set_len):
			data_to_write += (data_set[i])
			fwrite_txt = "Entry written ... %d/%d [%.2f %%]\r" % \
				(i+1, data_set_len, 100*(i+1)/data_set_len)
			sys.stdout.write(fwrite_txt)
			sys.stdout.flush()
		fp.write(data_to_write)

	print("")
	print("Written", n_images, "random pseudo image data.")

	tarf_name = ''.join(os.path.basename(dataf_name).split(".")[:-1])+'.tar.bz2'
	print("Making a tarball: %s" % tarf_name)
	with tarfile.open(tarf_name, "w:bz2") as tar:
		tar.add(dataf_name)

	print("Deleting %s" % dataf_name)
	os.remove(dataf_name)
	print("Datafile generation has been finished!!")


if __name__ == "__main__":
	main();
