CC=gcc
CFLAGS=-lzmq

SOURCES=$(wildcard *.c *.h)
OBJECTS=
TARGETS=hello_client hello_server weather_client weather_server \
	tasksink taskvent taskwork

all: $(TARGETS)

$(TARGETS): $(SOURCES)
	$(CC) $(CFLAGS) hello_server.c -o hello_server
	$(CC) $(CFLAGS) hello_client.c -o hello_client
	$(CC) $(CFLAGS) weather_server.c -o weather_server
	$(CC) $(CFLAGS) weather_client.c -o weather_client
	$(CC) $(CFLAGS) taskvent.c -o taskvent
	$(CC) $(CFLAGS) tasksink.c -o tasksink
	$(CC) $(CFLAGS) taskwork.c -o taskwork

clean:
	rm -rfv $(TARGETS)
