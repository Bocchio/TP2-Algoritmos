#ifndef GPS__H
#define GPS__H

#include <stdio.h>
#include "types.h"
#include "vector.h"

#define KML_HEADER_FILE		"kml_header.txt"
#define KML_FOOTER_FILE 	"kml_footer.txt"
#define KML_COORDINATES_TAG	"coordinates"

#define NMEA_GGA_LABEL	"coordinates"
#define NMEA_FIELD_DELIMITER	","

#define NMEA_ID_FIELD_INDEX	0

#define GGA_MSG_ID			"$GPGGA"
#define GGA_NORTH_TOKEN 	"N"
#define GGA_SOUTH_TOKEN 	"S"
#define GGA_EAST_TOKEN		"E"
#define GGA_WEST_TOKEN		"W"

/* field indexes in a GGA message */
#define GGA_EW_INDICATOR_FIELD_INDEX	5
#define GGA_NS_INDICATOR_FIELD_INDEX	3
#define GGA_LATITUDE_FIELD_INDEX		2
#define GGA_LONGITUDE_FIELD_INDEX		4
#define GGA_ALTITUDE_FIELD_INDEX		9

#define NMEA_CHECKSUM_DELIMITER	'*'
#define OUTPUT_CSV_DELIMITER	"|"
#define OUTPUT_KML_INDENTATION	4

status_t process_gps_file(FILE *fi, ADT_Vector_t *data, doc_type_t doc_type, FILE *fo);
status_t get_NMEA_message(string NMEA_message, uint *checksum);
status_t check_NMEA_message(const char * NMEA_message, uint checksum);

#endif
