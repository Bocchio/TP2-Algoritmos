#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "utils.h"
#include "nmea.h"
#include "gga.h"

status_t ADT_GGA_record_new(ADT_GGA_record_t **gga_record)
{
    if((*gga_record = (ADT_GGA_record_t *) malloc(sizeof(ADT_GGA_record_t))) == NULL){
        *gga_record = NULL;
        return ERROR_MEMORY;
    }

    (*gga_record)->latitude = 0;
    (*gga_record)->longitude = 0;
    (*gga_record)->altitude = 0;
    (*gga_record)->fix_quality = FIX_QUALITY_INVALID;

    return OK;
}

status_t ADT_GGA_record_destroy(ADT_GGA_record_t **gga_record)
{
    free(*gga_record);
    *gga_record = NULL;

    return OK;
}

status_t ADT_GGA_record_new_from_string(ADT_GGA_record_t **gga_record, string gga_message)
{
    status_t st;
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

    if((st = ADT_GGA_record_new(gga_record)) != OK){
        return st;
    }

    if((st = GGA_parse_fix_quality(fields[GGA_FIX_QUALITY_FIELD_INDEX],
                                   &((*gga_record)->fix_quality))) != OK){
        return st;
    }

    /* If the fix quality indicator is invalid, there isn't anything
     * useful to parse */
    if((*gga_record)->fix_quality == FIX_QUALITY_INVALID){
        return OK;
    }

    if((st = GGA_parse_latitude(fields[GGA_LATITUDE_FIELD_INDEX],
                                &((*gga_record)->latitude))) != OK){
        return st;
    }

    if((st = GGA_parse_longitude(fields[GGA_LONGITUDE_FIELD_INDEX],
                                 &((*gga_record)->longitude))) != OK){
        return st;
    }

     if((st = GGA_parse_altitude(fields[GGA_ALTITUDE_FIELD_INDEX],
                                 &((*gga_record)->altitude))) != OK){
        return st;
    }

    if(!strcmp(fields[GGA_NS_INDICATOR_FIELD_INDEX], GGA_SOUTH_TOKEN)){
        (*gga_record)->latitude = -(*gga_record)->latitude;
    }

    if(!strcmp(fields[GGA_EW_INDICATOR_FIELD_INDEX], GGA_WEST_TOKEN)){
        (*gga_record)->longitude = -(*gga_record)->longitude;
    }

    return OK;
}

status_t ADT_GGA_record_export_as_kml(const ADT_GGA_record_t *gga_record, const void *context, FILE *fo)
{
    if(gga_record == NULL || fo == NULL)
        return ERROR_NULL_POINTER;

    /* Print the data */
    if(fprintf(fo, "%f%c%f%c%i\n", gga_record->longitude, ',',
                                   gga_record->latitude, ',',
                                   (int) gga_record->altitude) < 0)
        return ERROR_WRITING_OUTPUT_FILE;

    return OK;
}

status_t ADT_GGA_record_export_as_csv(const ADT_GGA_record_t *gga_record, const string delimiter, FILE *fo)
{
    if(gga_record == NULL || delimiter == NULL || fo == NULL)
        return ERROR_NULL_POINTER;

    if(fprintf(fo, "%f%s%f%s%i\n", gga_record->latitude, delimiter,
                                   gga_record->longitude, delimiter,
                                   (int) gga_record->altitude) < 0)
        return ERROR_WRITING_OUTPUT_FILE;

    return OK;
}

/* Checks if a GGA record contains valid GPS data */
bool_t ADT_GGA_record_is_valid(ADT_GGA_record_t *gga_record)
{
    if(gga_record->fix_quality == FIX_QUALITY_INVALID)
        return FALSE;

    return TRUE;
}

/* Helper functions */

/* parse a GGA latitude string, assumes the string isn't empty */
status_t GGA_parse_latitude(string latitude_string, double *latitude)
{
    char *tmp;

    if(latitude_string == NULL || latitude == NULL){
        return ERROR_NULL_POINTER;
    }

    /* 60 minutes in a degree */
    *latitude += strtod(latitude_string+2, &tmp)/60.0;
    if(*tmp)
        return ERROR_PARSING_LATITUDE;

    latitude_string[2] = '\0';
    *latitude += strtod(latitude_string, &tmp);
    if(*tmp)
        return ERROR_PARSING_LATITUDE;

    return OK;
}

/* parse a GGA longitude string, assumes the string isn't empty */
status_t GGA_parse_longitude(string longitude_string, double *longitude)
{
    char *tmp;

    if(longitude_string == NULL || longitude == NULL){
        return ERROR_NULL_POINTER;
    }

    /* 60 minutes in a degree */
    *longitude = strtod(longitude_string+3, &tmp)/60.0;
    if(*tmp)
        return ERROR_PARSING_LONGITUDE;

    longitude_string[3] = '\0';
    *longitude += strtod(longitude_string, &tmp);
    if(*tmp)
        return ERROR_PARSING_LONGITUDE;

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
        return ERROR_PARSING_ALTITUDE;

    return OK;
}

status_t GGA_parse_fix_quality(string fix_quality_string, fix_quality_t *fix_quality)
{
    if(fix_quality_string == NULL || fix_quality == NULL){
        return ERROR_NULL_POINTER;
    }

    if(!strcmp(FIX_QUALITY_INVALID_TOKEN, fix_quality_string)){
        *fix_quality = FIX_QUALITY_INVALID;
        return OK;
    }
    else if(!strcmp(FIX_QUALITY_GPS_FIX_TOKEN, fix_quality_string)){
        *fix_quality = FIX_QUALITY_GPS_FIX;
        return OK;
    }
    else if(!strcmp(FIX_QUALITY_DGPS_FIX_TOKEN, fix_quality_string)){
        *fix_quality = FIX_QUALITY_DGPS_FIX;
        return OK;
    }
    else if(!strcmp(FIX_QUALITY_PPS_FIX_TOKEN, fix_quality_string)){
        *fix_quality = FIX_QUALITY_PPS_FIX;
        return OK;
    }
    else if(!strcmp(FIX_QUALITY_REAL_TIME_KINEMATIC_TOKEN, fix_quality_string)){
        *fix_quality = FIX_QUALITY_REAL_TIME_KINEMATIC;
        return OK;
    }
    else if(!strcmp(FIX_QUALITY_FLOAT_RTK_TOKEN, fix_quality_string)){
        *fix_quality = FIX_QUALITY_FLOAT_RTK;
        return OK;
    }
    else if(!strcmp(FIX_QUALITY_ESTIMATED_TOKEN, fix_quality_string)){
        *fix_quality = FIX_QUALITY_ESTIMATED;
        return OK;
    }
    else if(!strcmp(FIX_QUALITY_MANUAL_INPUT_MODE_TOKEN, fix_quality_string)){
        *fix_quality = FIX_QUALITY_MANUAL_INPUT_MODE;
        return OK;
    }
    else if(!strcmp(FIX_QUALITY_SIMULATION_MODE_TOKEN, fix_quality_string)){
        *fix_quality = FIX_QUALITY_SIMULATION_MODE;
        return OK;
    }

    return ERROR_PARSING_FIX_QUALITY;
}
