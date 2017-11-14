#include <stdio.h>
#include "main.h"
#include "types.h"
#include "errors.h"
#include "vector.h"

#include "adt.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "config.h"
#include "nmea.h"
#include "gga.h"

int main(int argc, char *argv[])
{
	status_t st;
    config_t config;
    FILE *input,*output;
    ADT_Vector_t *ADT_vector;

    if((st=validate_arguments(argc,argv,&config))!=OK){
        show_error(st);
        return st;
    }
    if((input=fopen(config.input_file_path,"rt"))==NULL){
        show_error(ERROR_OPENING_FILE);
        return ERROR_OPENING_FILE;
    }
    if((output=fopen(config.output_file_path,"wt"))==NULL){
        fclose(intput);
        show_errror(ERROR_OPENING_FILE);
        return ERROR_OPENING_FILE;
    }
    if((st=parse_NMEA_from_CSV(input,&ADT_vector))!=OK)
    {
        fclose(input);
        if(fclose(output)==EOF)
            show_error(ERROR_WRITING_FILE);
        show_error(st);
        return st;
    }
    if((st=ADT_export_vector(ADT_vector))!=OK)
    {
        fclose(input);
         if(fclose(output)==EOF)
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
    file_format_t output_file_format;

    if(argv == NULL || config == NULL)
        return ERROR_NULL_POINTER;

    if(argc != MAX_ARGS)
        return ERROR_PROGRAM_INVOCATION;

    /* parse each argument */
    for(i = 1; i < MAX_ARGS-1; i += 2){
        if(!strcmp(argv[i], CMD_ARG_FILE_FORMAT_TOKEN)){
            if((st = parse_file_format(argv[i+1], &(config->file_format))) != OK)
                return st;
        }
        else if(!strcmp(argv[i], CMD_ARG_OUTPUT_FILE_TOKEN)){
            if((st = strdup(argv[i+1], &(config->output_file_path))) != OK)
                return st;
        }
        else
            return ERROR_PROGRAM_INVOCATION;
    }
    if((st = strdup(argv[CMD_ARG_POSITION_INPUT_FILE_TOKEN], &(config->input_file_path))) != OK)
          return st;
	return OK;
}