#include <stdio.h>
#include <string.h>
#include "main.h"
#include "types.h"
#include "errors.h"
#include "config.h"
#include "vector.h"
#include "nmea.h"

int main(int argc, char *argv[])
{
	status_t st;
    config_t config;
    FILE *fi,*fo;
    ADT_Vector_t *ADT_vector;

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

    if((st = parse_NMEA_from_csv(fi, &ADT_vector)) != OK){
        fclose(fi);
        if(fclose(fo) == EOF)
            show_error(ERROR_WRITING_FILE);
        show_error(st);
        return st;
    }

    if((st = export_NMEA(ADT_vector, config.file_format, fo)) != OK){
        fclose(fi);
        if(fclose(fo) == EOF)
            show_error(ERROR_WRITING_FILE);
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
            if(!strcmp(format_string, FILE_FORMAT_KML))
                config->file_format = FORMAT_KML;
            if(!strcmp(format_string, FILE_FORMAT_CSV))
                config->file_format = FORMAT_CSV;
            return ERROR_UNKNOWN_FILE_FORMAT;
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