#ifndef NMEA__H
#define NMEA__H

#define FILE_FORMAT_CSV		"csv"
#define FILE_FORMAT_KML		"kml"

#define INPUT_CSV_DELIMITER	","

#include <stdio.h>
#include "types.h"
#include "vector.h"

typedef enum {
	FORMAT_KML,
	FORMAT_CSV
} file_format_t;

status_t parse_NMEA(FILE *fi, ADT_Vector_t *gga);
status_t parse_file_format(const string, file_format_t *);

#endif