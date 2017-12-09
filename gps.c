#include <stdio.h>
#include "types.h"
#include "nmea.h"
#include "vector.h"

printer_t doc_type_exporting_functions[]={
printer_t doc_type_exporting_functions[] = {
    NMEA_export_as_csv,
    NMEA_export_as_kml
};

exporter_t NMEA_exporting_functions[] = {
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

status_t load_gga_data(FILE *fi, ADT_Vector_t **data)
{
    status_t st;
    uint checksum;
    bool_t eof = FALSE;
    string line;
    string *fields;
    size_t number_of_fields;
    ADT_NMEA_record_t *nmea_record;

    if((st = ADT_Vector_new(data)) != OK){
        return st;
    }

    if((st = ADT_Vector_set_csv_exporter(*data, ADT_NMEA_record_export_as_csv)) != OK){
        ADT_Vector_destroy(data);
        return st;
    }

    if((st = ADT_Vector_set_kml_exporter(*data, ADT_NMEA_record_export_as_kml)) != OK){
        ADT_Vector_destroy(data);
        return st;
    }

    if((st = ADT_Vector_set_label(*data, KML_COORDINATES_TAG)) != OK){
        ADT_Vector_destroy(data);
        return st;
    }

    /* Read every line of the file */
    while(eof == FALSE){
        if((st = readline(fi, &line, &eof)) != OK){
            ADT_Vector_destroy(data);
            return st;
        }

        if((st = get_NMEA_message(line, &checksum)) != OK){
            return st;
        }

        /* Create an array of fields from the line read */
        if((st = split(line, &fields, NMEA_FIELD_DELIMITER, &number_of_fields)) != OK){
            return st;
        }

        /* If it's a GGA message */
        if(!strcmp(fields[GPGGA_HEADER_FIELD_INDEX], GPGGA_HEADER)){
            /* Check if the line is not corrupt */
            if((st = check_NMEA_message(line, checksum)) != OK){
                free(line);
                line = NULL;
                free_string_array(&fields, number_of_fields);
                ADT_Vector_destroy(data);
                return st;
            }

            /* create the NMEA record */
            if((st = ADT_NMEA_record_new(&node, fields)) != OK){
                free_string_array(&fields, number_of_fields);
                ADT_NMEA_record_delete_fields(data);
                return st;
            }
            /* If the node is not NULL (i.e, it contained geografic information) */
            if(node != NULL){
                /* then append it into vector */
                if((st = ADT_Vector_append(*data, node)) != OK){
                    free_string_array(&fields, len_fields_array);
                    ADT_NMEA_record_delete_fields(data);
                    return st;
                }
            }
        }
        free(line);
        free_string_array(&fields, len_fields_array);
    }

    return OK;
}

status_t parse_NMEA_latitude(string coord, double *degrees, bool_t *is_empty)
{
    char *tmp;
    char *minutes_pos;

    *degrees = 0;

    if((minutes_pos = strchr(coord, '.')) == NULL){
        /* if coord isn't "" then the file is corrupt*/
        if(*coord)
            return ERROR_READING_FILE;
        *is_empty = TRUE;
        return OK;
    }
    minutes_pos -= 2;
    /* 60 minutes in a degree */
    *degrees += strtod(minutes_pos, &tmp)/60.0;
    if(*tmp)
        return ERROR_READING_FILE;

    *minutes_pos = '\0';
    *degrees += strtod(coord, &tmp);
    if(*tmp)
        return ERROR_READING_FILE;

    *is_empty = FALSE;

    return OK;
}

status_t parse_NMEA_longitude(string coord, double *degrees, bool_t *is_empty)
{
    char *tmp;
    char *minutes_pos;

    *degrees = 0;

    if((minutes_pos = strchr(coord, '.')) == NULL){
        /* if coord isn't "" then the file is corrupt*/
        if(*coord)
            return ERROR_READING_FILE;
        *is_empty = TRUE;
        return OK;
    }
    minutes_pos -= 2;
    /* 60 minutes in a degree */
    *degrees += strtod(minutes_pos, &tmp)/60.0;
    if(*tmp)
        return ERROR_READING_FILE;

    *minutes_pos = '\0';
    *degrees += strtod(coord, &tmp);
    if(*tmp)
        return ERROR_READING_FILE;

    *is_empty = FALSE;

    return OK;
}
