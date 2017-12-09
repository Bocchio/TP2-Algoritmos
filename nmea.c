#include <stdio.h>
#include "nmea.h"

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

/* Makes a checksum of a NMEA message */
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
 status_t NMEA_export_as_csv(const ADT_Vector_t *vector, FILE *fo)
 {  
     status_t st;
     
      if(vector == NULL || fo == NULL)
        return ERROR_NULL_POINTER;
      if((st = ADT_GGA_record_export_as_csv(vector, OUTPUT_CSV_DELIMITER, fo)) != OK)
        return st;

    GGA_destroy_csv_context(&context);

    return OK;
}

status_t NMEA_export_as_kml(const ADT_Vector_t *vector, FILE *fo)
{
    status_t st;

    if(vector == NULL)
        return ERROR_NULL_POINTER;

    if((st = GGA_get_kml_ctx(&context)) != OK)
        return st;

    if((st = ADT_GGA_record_export_as_kml(vector, context, fo)) != OK)
        return st;

    GGA_destroy_kml_context(&context);

    return OK;
}
