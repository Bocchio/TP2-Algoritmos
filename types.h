#ifndef TYPES__H
#define TYPES__H

#include <stdio.h>

typedef enum {
    OK,
    ERROR_NULL_POINTER,
    ERROR_MEMORY,
    ERROR_UNKNOWN_FILE_FORMAT,
    ERROR_INVALID_LINE,
    ERROR_OPENING_NMEA_FILE,
    ERROR_OPENING_CONFIG_FILE,
    ERROR_OPENING_OUTPUT_FILE,
    ERROR_READING_NMEA_FILE,
    ERROR_READING_CONFIG_FILE,
    ERROR_WRITING_OUTPUT_FILE,
    ERROR_PROGRAM_INVOCATION,
    ERROR_CORRUPT_NMEA_FILE,
    ERROR_PARSING_LATITUDE,
    ERROR_PARSING_LONGITUDE,
    ERROR_PARSING_ALTITUDE,
    ERROR_PARSING_FIX_QUALITY
} status_t;

typedef enum {
    FALSE,
    TRUE
} bool_t;

typedef unsigned char uchar;
typedef unsigned int uint;
typedef char * string;

typedef enum {
    DOC_TYPE_CSV,
    DOC_TYPE_KML
} doc_type_t;

typedef status_t (*destructor_t)(void **);
typedef void * (*clonator_t)(const void *);
typedef status_t (*printer_t)(const void *, void *context, FILE *fo);
typedef status_t (*creator_t)(void **);

typedef struct{
    FILE *header;
    FILE *footer;
} kml_context_t;

#endif
