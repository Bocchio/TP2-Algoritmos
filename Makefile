CC = gcc
CFLAGS = -ansi -Wall -pedantic -g
LANGUAGE = -DENGLISH

all: gpsviewer clean

gpsviewer: main.o vector.o errors.o utils.o nmea.o config.o processor.o
	$(CC) $^ -o $@

vector.o: vector.c vector.h utils.h types.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<

utils.o: utils.c utils.h types.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<

errors.o: errors.c errors.h types.h lang_support.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<

nmea.o: nmea.c nmea.h utils.h types.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<

main.o: main.c main.h types.h errors.h config.h utils.h vector.h nmea.h processor.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<

config.o: config.c config.h types.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<
	
processor.o: processor.c processor.h types.h nmea.h vector.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) -o $@ $<
	
clean:
	rm -f *.o 

