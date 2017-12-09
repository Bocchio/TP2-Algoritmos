#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "errors.h"
#include "gps.h"
#include "config.h"
#include "main.h"

extern config_t config;

int main(int argc, char *argv[])
{
    status_t st;
    FILE *fi, *fo;

    if((st = validate_arguments(argc, argv, &config)) != OK){
        show_error(st);
        return st;
    }

    if((fi = fopen(config.output_file, "rt")) == NULL){
        st = ERROR_OPENING_NMEA_FILE;
        show_error(st);
        return st;
    }

    if((fo = fopen(config.input_file, "wt")) == NULL){
        fclose(fi);
        st = ERROR_OPENING_OUTPUT_FILE;
        show_error(st);
        return st;
    }

    if((st = process_gps_file(fi, config.output_doc_type, fo)) != OK){
        fclose(fi);
        if(fclose(fo) == EOF)
            show_error(ERROR_WRITING_OUTPUT_FILE);
        show_error(st);
        return st;
    }

    fclose(fi);
    if(fclose(fo) == EOF){
        st = ERROR_WRITING_OUTPUT_FILE;
        show_error(st);
        return st;
    }

    return OK;
}

status_t validate_arguments(int argc, char *argv[], config_t *config)
{
    size_t i;

    if(argv == NULL || config == NULL)
        return ERROR_NULL_POINTER;

    if(argc != MAX_ARGS)
        return ERROR_PROGRAM_INVOCATION;

    /* Parse each argument, takes into account that the last argument is positional */
    for(i = 1; i < MAX_ARGS-1; i += 2){
        /* Parse the output file format */
        if(!strcmp(argv[i], CMD_ARG_OUTPUT_FILE_FORMAT_TOKEN)){
            /* Get the appropriate doc type */
            if(!strcmp(argv[i+1], OUTPUT_DOC_TYPE_CSV_FLAG)){
                config->output_doc_type = DOC_TYPE_CSV;
            }
            else if(!strcmp(argv[i+1], OUTPUT_DOC_TYPE_KML_FLAG)){
                config->output_doc_type = DOC_TYPE_KML;
            }
            else{
                return ERROR_UNKNOWN_FILE_FORMAT;
            }
        }
        /* Parse the output file path */
        else if(!strcmp(argv[i], CMD_ARG_OUTPUT_FILE_TOKEN)){
            config->output_file = argv[i+1];
        }
        else
            return ERROR_PROGRAM_INVOCATION;
    }
    /* Parse the input file */
    config->input_file = argv[CMD_ARG_INPUT_FILE_POS];

    return OK;
}
