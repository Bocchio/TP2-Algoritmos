#ifndef NMEA__H
#define NMEA__H

#include <stdio.h>
#include "types.h"

#define NMEA_CHECKSUM_DELIMITER '*'
#define NMEA_FIELD_DELIMITER    ","

status_t get_NMEA_message(string NMEA_message, uint *checksum);
status_t check_NMEA_message(const char * NMEA_message, uint checksum);

#endif
