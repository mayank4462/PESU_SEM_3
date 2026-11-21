CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic

all: client.exe

client.exe: main.o function.o heap.o
	$(CC) $(CFLAGS) main.o function.o heap.o -o client.exe

main.o: main.c function.h heap.h
	$(CC) $(CFLAGS) -c main.c

function.o: function.c function.h
	$(CC) $(CFLAGS) -c function.c

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

clean:
	rm -f *.o client.exe
