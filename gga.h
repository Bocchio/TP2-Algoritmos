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
#define GGA_FIX_QUALITY_FIELD_INDEX     6

#define FIX_QUALITY_INVALID_TOKEN               "0"
#define FIX_QUALITY_GPS_FIX_TOKEN               "1"
#define FIX_QUALITY_DGPS_FIX_TOKEN              "2"
#define FIX_QUALITY_PPS_FIX_TOKEN               "3"
#define FIX_QUALITY_REAL_TIME_KINEMATIC_TOKEN   "4"
#define FIX_QUALITY_FLOAT_RTK_TOKEN             "5"
#define FIX_QUALITY_ESTIMATED_TOKEN             "6"
#define FIX_QUALITY_MANUAL_INPUT_MODE_TOKEN     "7"
#define FIX_QUALITY_SIMULATION_MODE_TOKEN       "8"

typedef enum {
    FIX_QUALITY_INVALID = 0,
    FIX_QUALITY_GPS_FIX = 1,
    FIX_QUALITY_DGPS_FIX = 2,
    FIX_QUALITY_PPS_FIX = 3,
    FIX_QUALITY_REAL_TIME_KINEMATIC = 4,
    FIX_QUALITY_FLOAT_RTK = 5,
    FIX_QUALITY_ESTIMATED = 6,
    FIX_QUALITY_MANUAL_INPUT_MODE = 7,
    FIX_QUALITY_SIMULATION_MODE = 8
} fix_quality_t;

typedef struct{
	double latitude;
	double longitude;
	double altitude;
    fix_quality_t fix_quality;
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

/* helper functions */

/* helper functions to parse geographic data
 * they assume that the strings are not empty */
status_t GGA_parse_latitude(string latitude_string, double *latitude);
status_t GGA_parse_longitude(string longitude_string, double *longitude);
status_t GGA_parse_altitude(string altitude_string, double *altitude);

/* helper function to get the fix quality from a string */
status_t GGA_parse_fix_quality(string fix_quality_string, fix_quality_t *fix_quality);



#endif
