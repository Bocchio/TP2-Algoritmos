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
