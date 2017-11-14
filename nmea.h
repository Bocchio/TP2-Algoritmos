#ifndef NMEA__H
#define NMEA__H

#define FILE_FORMAT_CSV		"csv"
#define FILE_FORMAT_KML		"kml"

#include <stdio.h>
#include "types.h"
#include "adt.h"
#include "vector.h"

typedef enum {
	FORMAT_KML,
	FORMAT_CSV
} file_format_t;

typedef struct{
	double latitude;
	double longitude;
	double altitude; 
	functions_interface_t functions_interface; 
} ADT_NMEA_GGA_t;

status_t parse_NMEA_from_csv(FILE *fi, ADT_Vector_t *gga, string delimiter);
status_t parse_file_format(const string, file_format_t *);

status_t ADT_NMEA_GGA_new(ADT_NMEA_GGA_t **nmea_gga_node, string *fields);
status_t ADT_NMEA_GGA_delete(ADT_NMEA_GGA_t **nmea_gga_node);

#endif