#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "utils.h"
#include "nmea.h"
#include "gga.h"

status_t ADT_GGA_record_new(ADT_GGA_record_t **gga_record)
{

    if((*gga_record = (ADT_GGA_record_t *) malloc(sizeof(ADT_GGA_record_t)) == NULL)){
        return ERROR_MEMORY;
    }

    (*gga_record)->latitude = 0;
    (*gga_record)->longitude = 0;
    (*gga_record)->altitude = 0;

    return OK;
}

status_t ADT_GGA_record_new_from_string(ADT_GGA_record_t **gga_record, string gga_message)
{
    uint checksum;
    string *fields;
    size_t number_of_fields;

    if((st = get_NMEA_message(gga_message, &checksum)) != OK){
        return st;
    }

    if((st = check_NMEA_message(gga_message, checksum)) != OK){
        return st;
    }

    if((st = split(gga_message, &fields, NMEA_FIELD_DELIMITER, &number_of_fields)) != OK){
        return st;
    }

    if((st = ADT_GGA_record_new_from_strings(gga_record, fields)) != OK){
        free_string_array(&fields, number_of_fields);
        return st;
    }

    return OK;
}

status_t ADT_GGA_record_new_from_strings(ADT_GGA_record_t **gga_record, string *fields)
{
    status_t st;
    bool_t is_empty;
    char *tmp;

    if((*gga_record = (ADT_GGA_GGA_t *) malloc(sizeof(ADT_GGA_GGA_t))) == NULL)
        return ERROR_MEMORY;

    if((st = ADT_GGA_record_parse_latitude(fields[GPGGA_LON_FIELD_INDEX], &((*gga_record)->longitude), &is_empty)) != OK){
        ADT_GGA_record_delete_fields(gga_record);
        return st;
    }

    if((st = ADT_GGA_record_parse_longitude(fields[GPGGA_LAT_FIELD_INDEX], &((*gga_record)->latitude), &is_empty)) != OK){
        ADT_GGA_record_delete_fields(gga_record);
        return st;
    }

    if(is_empty == TRUE){
        ADT_GGA_record_delete_fields(gga_record);
        return OK;
    }

    (*gga_record)->altitude = strtod(fields[GPGGA_ALT_FIELD_INDEX], &tmp);
    if(*tmp){
        ADT_GGA_record_delete_fields(gga_record);
        return ERROR_READING_FILE;
    }

    if(!strcmp(fields[GPGGA_NS_INDICATOR_POS], GPGGA_SOUTH_TOKEN)){
        (*gga_record)->latitude *= -1;
    }
    else if(strcmp(fields[GPGGA_NS_INDICATOR_POS], GPGGA_NORTH_TOKEN)){
        ADT_GGA_record_delete_fields(gga_record);
        return ERROR_READING_FILE;
    }

    if(!strcmp(fields[GPGGA_EW_INDICATOR_POS], GPGGA_WEST_TOKEN)){
        (*gga_record)->longitude *= -1;
    }
    else if(strcmp(fields[GPGGA_EW_INDICATOR_POS], GPGGA_EAST_TOKEN)){
        ADT_GGA_record_delete_fields(gga_record);
        return ERROR_READING_FILE;
    }

    return OK;
}

status_t ADT_GGA_record_export_as_kml(const ADT_GGA_record_t *gga, void *_ctx, FILE *fo)
{
    uchar i;
    xml_context_t *context;

    if(gga == NULL || _ctx == NULL || fo == NULL)
        return ERROR_NULL_POINTER;

    ctx = (xml_context_t *) _ctx;

    /* Tabulate the line*/
    for(i = 0; i < ctx->indentation; i++){
        if(fputc('\t', fo) == EOF)
            return ERROR_WRITING_FILE;
    }
    /* Print the data */
    if(fprintf(fo, "%f%c%f%c%i\n", gga->longitude, ',', gga->latitude, ',', (int) gga->altitude) < 0)
        return ERROR_WRITING_FILE;

    return OK;
}

status_t ADT_GGA_record_export_as_csv(const ADT_GGA_record_t *gga, void *ctx, FILE *fo)
{
    string delim;

    if(gga == NULL || ctx == NULL || fo == NULL)
        return ERROR_NULL_POINTER;

    delim = ctx;
    if(fprintf(fo, "%f%s%f%s%i\n", gga->longitude, delim, gga->latitude, delim, (int) gga->altitude) < 0)
        return ERROR_WRITING_FILE;

    return OK;
}

status_t GGA_get_kml_ctx(void **ctx)
{
    status_t st;
    FILE *header_file;
    FILE *footer_file;
    xml_context_t *kml_ctx;

    if(ctx == NULL)
        return ERROR_NULL_POINTER;

    if((kml_ctx = (xml_ctx_t *) malloc(sizeof(xml_ctx_t))) == NULL)
        return ERROR_MEMORY;

    if((header_file = fopen(KML_HEADER_FILE_PATH, "rt")) == NULL){
        GGA_destroy_kml_ctx((void **) &kml_ctx);
        return ERROR_OPENING_FILE;
    }

    if((footer_file = fopen(KML_FOOTER_FILE_PATH, "rt")) == NULL){
        GGA_destroy_kml_ctx((void **) &kml_ctx);
        return ERROR_OPENING_FILE;
    }

    /* get the header */
    if((st = readfile(header_file, &(kml_ctx->header))) != OK){
        GGA_destroy_kml_ctx((void **) &kml_ctx);
        return st;
    }

    /* get the footer */
    if((st = readfile(footer_file, &(kml_ctx->footer))) != OK){
        GGA_destroy_kml_ctx((void **) &kml_ctx);
        return st;
    }

    fclose(header_file);
    fclose(footer_file);

    kml_ctx->indentation = OUTPUT_KML_INDENTATION;

    *ctx = kml_ctx;

    return OK;
}


/* Helper functions */
/* parse a GGA latitude string, assumes the string isn't empty */
status_t GGA_parse_latitude(string latitude_string, double *latitude)
{
    char *tmp;
    char *minutes_pos;

    if(latitude_string == NULL || latitude == NULL){
        return ERROR_NULL_POINTER;
    }

    *latitude = 0;
    if((minutes_pos = strchr(latitude_string, '.')) == NULL){
        return ERROR_READING_INPUT_FILE;
    }

    minutes_pos -= 2;
    /* 60 minutes in a degree */
    *latitude += strtod(minutes_pos, &tmp)/60.0;
    if(*tmp)
        return ERROR_READING_INPUT_FILE;

    *minutes_pos = '\0';
    *latitude += strtod(latitude_string, &tmp);
    if(*tmp)
        return ERROR_READING_INPUT_FILE;

    return OK;
}

/* parse a GGA longitude string, assumes the string isn't empty */
status_t GGA_parse_longitude(string longitude_string, double *longitude)
{
    char *tmp;
    char *minutes_pos;

    if(longitude_string == NULL || longitude == NULL){
        return ERROR_NULL_POINTER;
    }

    *longitude = 0;
    if((minutes_pos = strchr(longitude_string, '.')) == NULL){
        return ERROR_READING_INPUT_FILE;
    }

    minutes_pos -= 2;
    /* 60 minutes in a degree */
    *longitude += strtod(minutes_pos, &tmp)/60.0;
    if(*tmp)
        return ERROR_READING_INPUT_FILE;

    *minutes_pos = '\0';
    *longitude += strtod(longitude_string, &tmp);
    if(*tmp)
        return ERROR_READING_INPUT_FILE;

    return OK;
}


/* parse a GGA altitude string, assumes the string isn't empty */
status_t GGA_parse_altitude(string altitude_string, double *altitude)
{
    char *tmp;

    if(altitude_string == NULL || altitude == NULL){
        return ERROR_NULL_POINTER;
    }

    *altitude = strtod(altitude_string, &tmp);
    if(*tmp)
        return ERROR_READING_INPUT_FILE;

    return OK;
}
