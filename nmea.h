#ifndef NMEA__H
#define NMEA__H

#include <stdio.h>
#include "types.h"

typedef struct{
	double latitude;
	double longitude;
	double altitude;
} ADT_NMEA_record_t;

status_t NMEA_export_as_csv(const ADT_Vector_t *vector, void *context, FILE *fo);
status_t NMEA_export_as_kml(const ADT_Vector_t *vector, void * context, FILE *fo);

status_t ADT_NMEA_record_parse_latitude(string coord, double *degrees, bool_t *is_empty);
status_t ADT_NMEA_record_parse_longitude(string coord, double *degrees, bool_t *is_empty);
status_t ADT_NMEA_record_new_from_strings(ADT_NMEA_record_t **nmea_record, string *fields);
status_t ADT_NMEA_record_delete_fields(ADT_NMEA_record_t **nmea_record);
status_t ADT_NMEA_record_load_fields(FILE *fi, ADT_Vector_t **gga);
status_t ADT_NMEA_record_export_as_kml(const ADT_NMEA_record_t *nmea_record, void *ctx, FILE *fo);
status_t ADT_NMEA_record_export_as_csv(const ADT_NMEA_record_t *nmea_record, void *ctx, FILE *fo);

status_t NMEA_get_kml_ctx(void **ctx);
status_t NMEA_destroy_kml_ctx(void **_ctx);

#endif
