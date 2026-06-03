CC = g++
CFLAGS = -Wall

all: server client

server: server.cpp
	$(CC) $(CFLAGS) server.cpp -o server -pthread

client: client.cpp
	$(CC) $(CFLAGS) client.cpp -o client -pthread

clean:
	rm -f server client
