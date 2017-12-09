#ifndef GGA__H
#define GGA__H

#include <stdio.h>
#include "types.h"

#define GGA_NORTH_TOKEN     "N"
#define GGA_SOUTH_TOKEN     "S"
#define GGA_EAST_TOKEN      "E"
#define GGA_WEST_TOKEN      "W"

/* field indexes in a GGA message */
#define GGA_EW_INDICATOR_FIELD_INDEX    5
#define GGA_NS_INDICATOR_FIELD_INDEX    3
#define GGA_LATITUDE_FIELD_INDEX        2
#define GGA_LONGITUDE_FIELD_INDEX       4
#define GGA_ALTITUDE_FIELD_INDEX        9

typedef struct{
	double latitude;
	double longitude;
	double altitude;
    uchar fix_quality;
} ADT_GGA_record_t;

/* constructors */
status_t ADT_GGA_record_new(ADT_GGA_record_t **gga_record);
status_t ADT_GGA_record_new_from_string(ADT_GGA_record_t **gga_record, string gga_message);
status_t ADT_GGA_record_new_from_strings(ADT_GGA_record_t **gga_record, string *gga_fields);

/* destructor */
status_t ADT_GGA_record_destroy(ADT_GGA_record_t **gga_record);

/* setters */
status_t ADT_GGA_record_set_latitude(ADT_GGA_record_t *gga_record, double latitude);
status_t ADT_GGA_record_set_longitude(ADT_GGA_record_t *gga_record, double longitude);
status_t ADT_GGA_record_set_altitude(ADT_GGA_record_t *gga_record, double altitude);

status_t GGA_export_as_csv(const ADT_Vector_t *vector, void *context, FILE *fo);
status_t GGA_export_as_kml(const ADT_Vector_t *vector, void * context, FILE *fo);

status_t ADT_GGA_record_export_as_kml(const ADT_GGA_record_t *gga_record, void *context, FILE *fo);
status_t ADT_GGA_record_export_as_csv(const ADT_GGA_record_t *gga_record, void *context, FILE *fo);

/* helper functions to parse geographic data
 * they assume that the strings are not empty */
status_t GGA_parse_latitude(string latitude_string, double *latitude);
status_t GGA_parse_longitude(string longitude_string, double *longitude);
status_t GGA_parse_altitude(string altitude_string, double *altitude);


#endif
