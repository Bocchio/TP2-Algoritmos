#include <stdio.h>
#include "types.h"
#include "nmea.h"
#include "vector.h"

printer_t doc_type_exporting_functions[]={
  NMEA_export_as_csv,
  NMEA_export_as_kml
};

void *context;

status_t process_gps_file(FILE *fi, FILE *fo, ADT_Vector_t *gga_vector, doc_type_t doc_type)
{
  status_t st;

  if(gga_vector == NULL || fi == NULL || fo == NULL)
       return ERROR_NULL_POINTER;

    if((st = ADT_NMEA_record_load_fields(fi, &gga_vector)) != OK)
        return st;
    if((st = doc_type_exporting_functions[doc_type](gga_vector,context,fo)) != OK)
        return st;
    return OK;
}

/* takes a line from a NMEA file and extracts the message and the checksum */
status_t get_NMEA_message(string NMEA_message, uint *checksum)
{
    size_t i;
    char *tmp;

    if(NMEA_message == NULL || checksum == NULL){
        return ERROR_NULL_POINTER;
    }

    for(; *NMEA_message; NMEA_message++){
        if(*NMEA_message == NMEA_CHECKSUM_DELIMITER){
            *NMEA_message = '\0';
            /* Move to the beginning of the checksum */
            NMEA_message++;
            break;
        }
    }

    checksum = strtoul(NMEA_message, &tmp, 10);

    if(*tmp && tmp != '\r' && tmp != '\n'){
        return ERROR_INVALID_LINE;
    }

    return OK;
}

status_t check_NMEA_message(const char *NMEA_message, uint checksum)
{
    uint crc = 0;

    for(; *NMEA_message; NMEA_message++){
        crc ^= *NMEA_message;
    }

    if(crc != checksum){
        return ERROR_INVALID_LINE;
    }

    return OK;
}