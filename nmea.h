#ifndef NMEA__H
#define NMEA__H

#include "types.h"

typedef enum {
	FORMAT_KML,
	FORMAT_CSV
} file_format;

status_t parse_NMEA();
status_t parse_file_format(string *, file_format *);

#endif