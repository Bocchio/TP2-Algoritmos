#include <stdio.h>
#include <stdlib.h>
#include "nmea.h"

/* takes a line from a NMEA file and extracts the message and the checksum */
status_t get_NMEA_message(string NMEA_message, uint *checksum)
{
    char *tmp;

    if(NMEA_message == NULL || checksum == NULL){
        return ERROR_NULL_POINTER;
    }

    /* Go over the NMEA datagram looking for a checksum or its end */
    for(; *NMEA_message; NMEA_message++){
        /* If the checksum delimiter is found */
        if(*NMEA_message == NMEA_CHECKSUM_DELIMITER){
            /* Separate the NMEA message from the checksum */
            *NMEA_message = '\0';
            /* Move to the beginning of the checksum */
            NMEA_message++;
            break;
        }
    }

    /* Get the checksum value. Note that if there isn't one, it's just tries to
     * convert "", which strtoul interprets as 0 */
    *checksum = strtoul(NMEA_message, &tmp, 10);
    if(*tmp && *tmp != '\r' && *tmp != '\n'){
        return ERROR_INVALID_LINE;
    }

    return OK;
}

/* Makes a checksum of a NMEA message */
status_t check_NMEA_message(const char *NMEA_message, uint checksum)
{
    uint crc = 0;

    /* Calculate the CRC */
    for(; *NMEA_message; NMEA_message++){
        crc ^= *NMEA_message;
    }

    /* Check if it differs */
    if(crc != checksum){
        return ERROR_INVALID_LINE;
    }

    return OK;
}
