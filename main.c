#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "types.h"
#include "errors.h"
#include "config.h"
#include "utils.h"
#include "vector.h"
#include "nmea.h"

extern config;

int main(int argc, char *argv[])
{
	status_t st;
    FILE *fi,*fo;
    ADT_Vector_t *gga_vector;

    if((st = validate_arguments(argc, argv, &config)) != OK){
        show_error(st);
        return st;
    }

    if((fi = fopen(config.fi_path, "rt")) == NULL){
        st = ERROR_OPENING_FILE;
        show_error(st);
        return st;
    }

    if((fo = fopen(config.fo_path, "wt")) == NULL){
        fclose(fi);
        st = ERROR_OPENING_FILE;
        show_error(st);
        return st;
    }

   

	return OK;
}

status_t validate_arguments(int argc, char *argv[], config_t *config)
{
	size_t i;
    status_t st;

    if(argv == NULL || config == NULL)
        return ERROR_NULL_POINTER;

    if(argc != MAX_ARGS)
        return ERROR_PROGRAM_INVOCATION;

    /* parse each argument, takes into account that the last argument is positional */
    for(i = 1; i < MAX_ARGS-1; i += 2){
        if(!strcmp(argv[i], CMD_ARG_FILE_FORMAT_TOKEN)){
            if(!strcmp(argv[i+1], FILE_FORMAT_CSV_FLAG)){
                config->file_format = FORMAT_CSV;
            }
            else if(!strcmp(argv[i+1], FILE_FORMAT_KML_FLAG)){
                config->file_format = FORMAT_KML;
            }
            else{
                return ERROR_UNKNOWN_FILE_FORMAT;
            }
        }
        else if(!strcmp(argv[i], CMD_ARG_OUTPUT_FILE_TOKEN)){
            if((st = strdup(argv[i+1], &(config->fo_path))) != OK)
                return st;
        }
        else
            return ERROR_PROGRAM_INVOCATION;
    }
    if((st = strdup(argv[CMD_ARG_INPUT_FILE_POS], &(config->fi_path))) != OK){
        free(config->fo_path);
        return st;
    }

	return OK;
}
