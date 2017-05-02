CC=gcc
CFLAGS=-ggdb3
LIBS=-lzmq

SOURCES=$(wildcard *.c *.h)
OBJECTS=
TARGETS=hello_client hello_server weather_client weather_server \
	tasksink taskvent taskwork mspoller msreader mt_hello_server \
	interrupt mt_relay syncpub syncsub psenpub psensub rtdealer \
	czmq_hello

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

mt_relay: mt_relay.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS) -pthread

syncpub: syncpub.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

syncsub: syncsub.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

psenpub: psenpub.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

psensub: psensub.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

rtdealer: rtdealer.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS) -pthread

czmq_hello: czmq_hello.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS) -lczmq

clean:
	rm -rfv $(TARGETS)
