#include <stdio.h>
#include "types.h"
#include "nmea.h"
#include "vector.h"

printer_t printers[]={
  NMEA_export_as_csv,
  NMEA_export_as_kml
};

void *context;

status_t process_gps_file(FILE*fi,FILE*fo,ADT_Vector_t * gga_vector,file_format_t file_format){
  status_t st;
  
  
  if(gga_vector==NULL||fi==NULL||fo==NULL)
       return ERROR_NULL_POINTER;
  
   if((st = ADT_GGA_load_fields(fi, &gga_vector)) != OK)
        return st;
    if((st = printers[config.file_format](gga_vector,context,fo)) != OK)
        return st;
    return OK;
}
