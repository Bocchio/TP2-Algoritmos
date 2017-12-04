#include <stdio.h>
#include "types.h"
#include "nmea.h"
#include "vector.h"

status_t process_gps_file(FILE*fi,FILE*fo,ADT_Vector_t * gga_vector,file_format_t file_format){
  status_t st;
  
  
  if(gga_vector==NULL||fi==NULL||fo==NULL)
       return ERROR_NULL_POINTER;
  
   if((st = ADT_GGA_load_fields(fi, &gga_vector)) != OK)
        return st;
    if((st = ADT_GGA_export_fields(gga_vector, config.file_format, fo)) != OK)
        return st;
    return OK;
}
