#include <stdio.h>
#include "types.h"
#include "vector.h"
#include "gga.h"
#include "gps.h"

/* Dictionary of functions for exporting GGA data according to a doc type */
printer_t export_functions[] = {
    (printer_t) export_GGA_data_as_csv,
    (printer_t) export_GGA_data_as_kml
};

void *context;

status_t process_gps_file(FILE *fi, doc_type_t doc_type, FILE *fo)
{
    status_t st;
    ADT_Vector_t *gga_data;

    if(fi == NULL || fo == NULL){
        return ERROR_NULL_POINTER;
    }

    if((st = load_gga_data(fi, &gga_data)) != OK){
        return st;
    }

    if((st = export_functions[doc_type](gga_data)) != OK){
        return st;
    }

    return OK;
}

status_t load_GGA_data(FILE *fi, ADT_Vector_t **gga_data)
{
    status_t st;
    string line;
    ADT_GGA_record_t *gga_record;
    bool_t eof = FALSE;

    /* Create a vector of GGA records */
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

    /* For every line in the input file */
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

status_t export_GGA_data_as_csv(ADT_Vector_t *gga_data, void *context, FILE *fo)
{
    kml_context_t kml_context;

    if(gga_data == NULL || fo == NULL){
        return ERROR_NULL_POINTER;
    }

    /* Set the kml context */
    if((kml_context.header = fopen(KML_HEADER_FILE, "rt")) == NULL){
        return ERROR_OPENING_CONFIG_FILE;
    }
    if((kml_context.footer = fopen(KML_FOOTER_FILE, "rt")) == NULL){
        return ERROR_OPENING_CONFIG_FILE;
    }

    if((st = ADT_Vector_export_as_kml(gga_data, &kml_context, fo)) != OK){
        return st;
    }

    fclose(kml_context.header);
    fclose(kml_context.footer);

    return OK;
}

status_t export_GGA_data_as_kml(ADT_Vector_t *gga_data, void *context, FILE *fo)
{
    if(gga_data == NULL || fo == NULL){
        return ERROR_NULL_POINTER;
    }

    if((st = ADT_Vector_export_as_csv(gga_data, OUTPUT_CSV_DELIMITER, fo)) != OK){
        return st;
    }

    return OK;
}
