#include <stdio.h>
#include "main.h"
#include "types.h"
#include "errors.h"
#include "vector.h"

#include "adt.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main(int argc, char *argv[])
{
	status_t st;

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

    /* parse each argument */
    for(i = 1; i < MAX_ARGS; i += 2){
        if(!strcmp(argv[i], CMD_ARG_DATE_START_FORMAT_TOKEN)){
            if((st = get_date_format(argv[i+1], &date_format_start)) != OK)
                return st;
        }
        else if(!strcmp(argv[i], CMD_ARG_DATE_END_FORMAT_TOKEN)){
            if((st = get_date_format(argv[i+1], &date_format_end)) != OK)
                return st;
        }
        else if(!strcmp(argv[i], CMD_ARG_DATE_START_TOKEN)){
            if(MAX_DATE_STRING_LEN < strlen(argv[i+1]))
                return ERROR_INVALID_DATE;
            strcpy(date_start, argv[i+1]);
        }
        else if(!strcmp(argv[i], CMD_ARG_DATE_END_TOKEN)){
            if(MAX_DATE_STRING_LEN < strlen(argv[i+1]))
                return ERROR_INVALID_DATE;
            strcpy(date_end, argv[i+1]);
        }
        else if(!strcmp(argv[i], CMD_ARG_UNIT_FORMAT_TOKEN)){
            if((st = get_time_unit(argv[i+1], &(config->output_unit))) != OK)
                return st;
        }
        else
            return ERROR_PROGRAM_INVOCATION;
    }

    /* if everything was fine, convert the dates to time_t */
    if((st = date2time(date_start, date_format_start, &(config->date_start))) != OK)
        return st;
    if((st = date2time(date_end, date_format_end, &(config->date_end))) != OK)
        return st;    

	return OK;
}