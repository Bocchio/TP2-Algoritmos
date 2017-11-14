 CC = gcc
CFLAGS = -ansi -Wall -pedantic-errors
LANGUAGE = -DENGLISH
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

all: gpsviewer clean

gpsviewer: main.o vector.o errors.o utils.o
	$(CC) $^ -o $@

vector.o: vector.c
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<

utils.o: utils.c
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<

errors.o: errors.c
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<

main.o: main.c
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<

clean:
	rm -f *.o 

