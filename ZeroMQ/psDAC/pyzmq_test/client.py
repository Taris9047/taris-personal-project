#!/usr/bin/env python3
import os, zmq, time

context = zmq.Context()
pusher = context.socket(zmq.PUSH)
pusher.connect('tcp://localhost:13377')

message_size = 20*(1024**2)
message = 'x' * message_size

bit_rate_list = []
for i in range(10000):
	st_time = time.time()
	pusher.send(message.encode('utf-8'))
	end_time = time.time()
	print("Execution time: %d us"%((end_time-st_time)*1000000))
	print("Transfer rate: %d bps"%(message_size*8/(end_time-st_time)))
	bit_rate_list.append((message_size*8/(end_time-st_time)))

print("Average transfer rate: %d bps"%(sum(bit_rate_list)/len(bit_rate_list)))
