CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

OBJS = main.o encode.o

all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o res

main.o: main.c encode.h
	$(CC) $(CFLAGS) -c main.c

encode.o: encode.c encode.h
	$(CC) $(CFLAGS) -c encode.c

clean:
	rm -f $(OBJS) res
