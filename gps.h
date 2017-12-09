#ifndef GPS__H
#define GPS__H

#include <stdio.h>
#include "types.h"

#define KML_HEADER_FILE     "kml_header.txt"
#define KML_FOOTER_FILE     "kml_footer.txt"
#define KML_COORDINATES_TAG "coordinates"

#define NMEA_ID_FIELD_INDEX 0

#define GGA_HEADER  "$GPGGA"

#define OUTPUT_CSV_DELIMITER    "|"
#define OUTPUT_KML_INDENTATION  4

status_t process_gps_file(FILE *fi, doc_type_t doc_type, FILE *fo);

#endif
