CC=gcc
CFLAGS=-lzmq

SOURCES=$(wildcard *.c *.h)
OBJECTS=
TARGETS=hello_client hello_server weather_client weather_server \
	tasksink taskvent taskwork

all: $(TARGETS)

hello_client: hello_client.c
	$(CC) $(CFLAGS) $< -o $@

hello_server: hello_server.c
	$(CC) $(CFLAGS) $< -o $@

weather_server: weather_server.c
	$(CC) $(CFLAGS) $< -o $@

weather_client: weather_client.c
	$(CC) $(CFLAGS) $< -o $@

taskvent: taskvent.c
	$(CC) $(CFLAGS) $< -o $@

tasksink: tasksink.c
	$(CC) $(CFLAGS) $< -o $@

taskwork: taskwork.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rfv $(TARGETS)
