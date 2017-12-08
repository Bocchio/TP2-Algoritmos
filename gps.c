#include <stdio.h>
#include "types.h"
#include "nmea.h"
#include "vector.h"

printer_t doc_type_exporting_functions[]={
  NMEA_export_as_csv,
  NMEA_export_as_kml
};

void *context;

status_t process_gps_file(FILE *fi, doc_type_t doc_type, FILE *fo)
{
  status_t st;

    if(fi == NULL || fo == NULL){
        return ERROR_NULL_POINTER;
    }


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



/*status_t ADT_NMEA_record_load_fields(FILE *fi, ADT_Vector_t **gga_vector)
{
    status_t st;
    bool_t eof = FALSE;
    string line;
    string *fields;
    size_t len_fields_array;
    ADT_NMEA_GGA_t *node;

    if((st = ADT_Vector_new(gga_vector)) != OK){
        return st;
    }

    if((st = ADT_Vector_set_csv_exporter(*gga_vector, ADT_NMEA_record_export_as_csv)) != OK){
        ADT_NMEA_record_delete_fields(gga_vector);
        return st;
    }

    if((st = ADT_Vector_set_kml_exporter(*gga_vector, ADT_NMEA_record_export_as_kml)) != OK){
        ADT_NMEA_record_delete_fields(gga_vector);
        return st;
    }

    if((st = ADT_Vector_set_label(*gga_vector, KML_COORDINATES_TAG)) != OK){
        ADT_NMEA_record_delete_fields(gga_vector);
        return st;
    }

    /* Read every line of the file */
    while(eof == FALSE){
        if((st = readline(fi, &line, &eof)) != OK){
            return st;
        }
        /* Create an array of fields from the line read */
        if((st = split(line, &fields, NMEA_FIELD_DELIMITER, &len_fields_array)) != OK){
            return st;
        }
        free(line);

        /* If it's a GGA message */
        if(!strcmp(fields[GPGGA_HEADER_FIELD_INDEX], GPGGA_HEADER)){
            /* create the GGA node */
            if((st = ADT_NMEA_record_new(&node, fields)) != OK){
                free_string_array(&fields, len_fields_array);
                ADT_NMEA_record_delete_fields(gga_vector);
                return st;
            }
            /* If the node is not NULL (i.e, it contained geografic information) */
            if(node != NULL){
                /* then append it into vector */
                if((st = ADT_Vector_append(*gga_vector, node)) != OK){
                    free_string_array(&fields, len_fields_array);
                    ADT_NMEA_record_delete_fields(gga_vector);
                    return st;
                }
            }
        }
        free_string_array(&fields, len_fields_array);
    }

    return OK;
}*/