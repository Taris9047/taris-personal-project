#!/bin/sh

echo "Starting clients"

for ((i=0; i<10; i++)); do
	./hello_client &
done

echo "Starting server"
./mt_hello_server
