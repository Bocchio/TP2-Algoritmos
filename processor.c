#include <stdio.h>
#include "types.h"
#include "nmea.h"
#include "vector.h"

status_t process_gps_file(FILE*fi,FILE*fo,ADT_Vector_t * gga_vector,file_format_t file_format){
  status_t st;
  
  
  if(gga_vector==NULL||fi==NULL||fo==NULL)
       return ERROR_NULL_POINTER;
  
   if((st = parse_NMEA(fi, &gga_vector)) != OK)
        return st;
    if((st = export_NMEA(gga_vector, config.file_format, fo)) != OK)
        return st;
    return OK;
}
