CC=gcc
CFLAGS=-ggdb3
LIBS=-lzmq

SOURCES=$(wildcard *.c *.h)
OBJECTS=
TARGETS=hello_client hello_server weather_client weather_server \
	tasksink taskvent taskwork mspoller msreader mt_hello_server \
	interrupt

all: $(TARGETS)

hello_client: hello_client.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

hello_server: hello_server.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

weather_server: weather_server.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

weather_client: weather_client.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

taskvent: taskvent.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

tasksink: tasksink.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

taskwork: taskwork.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

mspoller: mspoller.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

msreader: msreader.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

mt_hello_server: mt_hello_server.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS) -pthread

interrupt: interrupt.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -rfv $(TARGETS)
