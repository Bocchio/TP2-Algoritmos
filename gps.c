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
    ADT_Vector_t *gga_data;

    if(fi == NULL || fo == NULL){
        return ERROR_NULL_POINTER;
    }




    return OK;
}

status_t load_gga_data(FILE *fi, ADT_Vector_t **gga_data)
{
    status_t st;
    string line;
    ADT_GGA_record_t *gga_record;
    bool_t eof = FALSE;


    if((st = ADT_Vector_new(gga_data)) != OK){
        return st;
    }

    if((st = ADT_Vector_set_csv_exporter(*gga_data, ADT_GGA_record_export_as_csv)) != OK){
        ADT_Vector_destroy(gga_data);
        return st;
    }

    if((st = ADT_Vector_set_kml_exporter(*gga_data, ADT_GGA_record_export_as_kml)) != OK){
        ADT_Vector_destroy(gga_data);
        return st;
    }

    if((st = ADT_Vector_set_tag(*gga_data, KML_COORDINATES_TAG)) != OK){
        ADT_Vector_destroy(gga_data);
        return st;
    }

    while(eof == FALSE){
        if((st = readline(fi, &line, &eof)) != OK){
            return st;
        }

        /* If it isn't a GGA message, discard it */
        if(starts_with(line, GGA_HEADER) == FALSE){
            free(line);
            continue;
        }

        /* Otherwise create a gga_record from it */
        if((st = ADT_GGA_record_new_from_string(&gga_record, line)) != OK){
            free(line);
            ADT_Vector_destroy(gga_data);
            return st;
        }
        free(line);

        /* If the record doesn't have valid gps data, discard it */
        if(ADT_GGA_record_is_valid(gga_record) == FALSE){
            ADT_GGA_record_destroy(&gga_record);
            continue;
        }
        /* Otherwise append it to the vector */
        if((st = ADT_Vector_append(*gga_data, gga_record)) != OK){
            ADT_GGA_record_destroy(&gga_record);
            ADT_Vector_destroy(gga_data);
            return st;
        }
    }

    return OK;
}
