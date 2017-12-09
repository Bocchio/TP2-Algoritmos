#include <stdio.h>
#include "types.h"
#include "nmea.h"
#include "vector.h"

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
    bool_t gga_header, gga_message_invalid;
    string line;
    string *fields;
    size_t number_of_fields;
    ADT_GGA_record_t * gga_record;


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

    while(eof == FALSE){
        if((st = readline(fi, &line, &eof)) != OK){
            return st;
        }

        /* If it is a GGA message */
        if((gga_header = starts_with(line, GPGGA_HEADER)) == FALSE)
            continue;
        if((st = ADT_GGA_record_new_from_string(&gga_record, line)) != OK){
            free(line);
            ADT_Vector_destroy(data);
            return st;
        }
        free(line);
        if(ADT_GGA_record_is_valid(gga_record) == FALSE)
             ADT_GGA_record_destroy(&gga_record);
        else {
            if((st = ADT_Vector_append(*data, gga_record)) != OK){
                ADT_GGA_record_destroy(&gga_record);
                ADT_Vector_destroy(data);
                return st;
            }
        }
    }

    return OK;
}
