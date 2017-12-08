CC = gcc
CFLAGS = -ansi -Wall -pedantic -g

all: test_utils clean

test_utils: test_utils.o minitest.o ../utils.o split_tests.o starts_with_tests.o
	$(CC) $(CFLAGS) $^ -o $@

test_utils.o: test_utils.c ../types.h ../utils.h
	$(CC) -c $(CFLAGS) $^

split_tests.o: split_tests.c split_tests.h
	$(CC) -c $(CFLAGS) $^

starts_with_tests.o: starts_with_tests.c starts_with_tests.h
	$(CC) -c $(CFLAGS) $^

../utils.o: ../utils.c
	$(CC) -c $(CFLAGS) $^ -o $@

minitest.o: minitest.c minitest.h
	$(CC) -c $(CFLAGS) $^

clean:
	rm -f *.o 
	rm -f ../*.o

