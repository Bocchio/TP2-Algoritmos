#ifndef NMEA__H
#define NMEA__H

#include <stdio.h>
#include "types.h"

typedef struct{
	double latitude;
	double longitude;
	double altitude;
} ADT_NMEA_record_t;

/* constructors */
status_t ADT_NMEA_record_new(ADT_NMEA_record_t **nmea_record);
status_t ADT_NMEA_record_new_from_strings(ADT_NMEA_record_t **nmea_record, string *fields);

/* destructor */
status_t ADT_NMEA_record_delete(ADT_NMEA_record_t **nmea_record);

/* setters */
status_t ADT_NMEA_record_set_latitude(ADT_NMEA_record_t *nmea_record, double latitude);
status_t ADT_NMEA_record_set_longitude(ADT_NMEA_record_t *nmea_record, double longitude);
status_t ADT_NMEA_record_set_altitude(ADT_NMEA_record_t *nmea_record, double altitude);

status_t NMEA_export_as_csv(const ADT_Vector_t *vector, void *context, FILE *fo);
status_t NMEA_export_as_kml(const ADT_Vector_t *vector, void * context, FILE *fo);


status_t ADT_NMEA_record_export_as_kml(const ADT_NMEA_record_t *nmea_record, void *context, FILE *fo);
status_t ADT_NMEA_record_export_as_csv(const ADT_NMEA_record_t *nmea_record, void *context, FILE *fo);

#endif
