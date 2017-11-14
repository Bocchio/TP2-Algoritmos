#ifndef NMEA__H
#define NMEA__H

<<<<<<< HEAD
=======
#include "types.h"

typedef enum {
	FORMAT_KML,
	FORMAT_CSV
} file_format_t;

status_t parse_NMEA();
status_t parse_file_format(string *, file_format_t *);

>>>>>>> 16e32f3b15c17a4f39fbc5dd24a2f17d7b361b06
#endif