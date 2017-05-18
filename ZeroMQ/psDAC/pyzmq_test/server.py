#!/usr/bin/env python3
import zmq

context = zmq.Context()
receiver = context.socket(zmq.PULL)
receiver.bind('tcp://*:13377')

while True:
	receiver.recv()
