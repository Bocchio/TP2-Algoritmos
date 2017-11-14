#ifndef NMEA__H
#define NMEA__H

#include <stdio.h>
#include "types.h"
#include "adt.h"
#include "vector.h"


#define FILE_FORMAT_CSV		"csv"
#define FILE_FORMAT_KML		"kml"

#define GPGA_HEADER	"$GPGGA"

#define KML_HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"

typedef enum {
	FORMAT_KML,
	FORMAT_CSV
} file_format_t;

typedef struct{
	double latitude;
	double longitude;
	double altitude;
} ADT_NMEA_GGA_t;

status_t parse_NMEA_from_csv(FILE *fi, ADT_Vector_t *gga, string delimiter);
status_t parse_file_format(const string, file_format_t *);
status_t export_NMEA(const ADT_Vector_t *vector, file_format_t format);

status_t ADT_NMEA_GGA_new(ADT_NMEA_GGA_t **nmea_gga_node, string *fields);
status_t ADT_NMEA_GGA_delete(ADT_NMEA_GGA_t **nmea_gga_node);

#endif