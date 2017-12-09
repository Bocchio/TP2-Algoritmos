CC = gcc
CFLAGS = -ansi -Wall -pedantic -g
LANGUAGE = -DENGLISH

all: gpsviewer clean

gpsviewer: main.o config.o errors.o utils.o vector.o gga.o nmea.o gps.o
	$(CC) $^ -o $@

gps.o: gps.c gps.h types.h vector.h gga.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) $<

nmea.o: nmea.c nmea.h types.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) $<

gga.o: gga.c gga.h types.h utils.h nmea.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) $<

vector.o: vector.c vector.h types.h utils.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) $<

errors.o: errors.c errors.h types.h lang_support.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) $<

main.o: main.c main.h types.h errors.h config.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) $<

config.o: config.c config.h types.h
	$(CC) -c $(CFLAGS) $(LANGUAGE) $<

clean:
	rm -f *.o

