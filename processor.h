#ifndef PROCESSOR__H
#define PROCESSOR__H
#include <stdio.h>
#include "types.h"
#include "nmea.h"
#include "vector.h"

status_t process_gps_file(FILE*fi,FILE*fo,ADT_Vector_t * gga_vector,doc_type_t doc_type);

#endif
