#ifndef NMEA__H
#define NMEA__H

#include <stdio.h>
#include "types.h"
#include "vector.h"

#define KML_HEADER_FILE	"kml_header.txt"
#define KML_FOOTER_FILE 	"kml_footer.txt"
#define KML_COORDINATES_TAG	"coordinates"

#define GPGGA_HEADER	"$GPGGA"
#define NMEA_GGA_LABEL	"coordinates"
#define NMEA_FIELD_DELIMITER	","

#define GPGGA_NORTH_TOKEN 	"N"
#define GPGGA_SOUTH_TOKEN 	"S"
#define GPGGA_EAST_TOKEN	"E"
#define GPGGA_WEST_TOKEN	"W"

#define GPGGA_EW_INDICATOR_POS	5
#define GPGGA_NS_INDICATOR_POS	3

#define GPGGA_LAT_POS		2 /* latitude position on a GPGGA string */
#define GPGGA_LON_POS		4 /* longitude position on a GPGGA string */
#define GPGGA_ALT_POS		9 /* altitude position on a GPGGA string */

#define OUTPUT_CSV_DELIMITER	"|"
#define OUTPUT_KML_INDENTATION	4

typedef struct{
	double latitude;
	double longitude;
	double altitude;
} ADT_GGA_t;

status_t parse_NMEA(FILE *fi, ADT_Vector_t **gga);
status_t export_NMEA(const ADT_Vector_t *vector, file_format_t format, FILE *fo);
status_t parse_NMEA_latitude(string coord, double *degrees, bool_t *is_empty);
status_t parse_NMEA_longitude(string coord, double *degrees, bool_t *is_empty);

status_t ADT_NMEA_GGA_new(ADT_GGA_t **gga_node, string *fields);
status_t ADT_NMEA_GGA_delete(ADT_GGA_t **gga_node);
status_t ADT_NMEA_GGA_export_as_kml(const ADT_GGA_t *gga_node, void *ctx, FILE *fo);
status_t ADT_NMEA_GGA_export_as_csv(const ADT_GGA_t *gga_node, void *ctx, FILE *fo);

status_t NMEA_get_csv_ctx(void **ctx);
status_t NMEA_get_kml_ctx(void **ctx);
status_t NMEA_destroy_csv_ctx(void **_ctx);
status_t NMEA_destroy_kml_ctx(void **_ctx);

#endif
