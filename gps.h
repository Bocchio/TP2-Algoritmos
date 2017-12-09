#ifndef GPS__H
#define GPS__H

#include <stdio.h>
#include "types.h"
#include "vector.h"

#define KML_HEADER_FILE     "kml_header.txt"
#define KML_FOOTER_FILE     "kml_footer.txt"
#define KML_COORDINATES_TAG "coordinates"

#define NMEA_ID_FIELD_INDEX 0

#define GGA_HEADER  "$GPGGA"

#define OUTPUT_CSV_DELIMITER    "|"

status_t process_gps_file(FILE *fi, doc_type_t doc_type, FILE *fo);
status_t export_GGA_data_as_csv(ADT_Vector_t *gga_data, void *context, FILE *fo);
status_t export_GGA_data_as_kml(ADT_Vector_t *gga_data, void *context, FILE *fo);
status_t load_GGA_data(FILE *fi, ADT_Vector_t **gga_data);

#endif
