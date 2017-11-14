#include <string.h>
#include <stdio.h>
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include "nmea.h"

status_t parse_NMEA_from_csv(FILE *fi, ADT_Vector_t **gga_vector, string delimiter)
{
	status_t st;
	size_t i;
	bool_t eof = FALSE;
	string line;
	string *fields;
	ADT_NMEA_GGA_t *node;

	if((st = ADT_Vector_new(gga_vector, )) != OK){
		return st;
	}

	while(eof == FALSE){
		if((st = readline(fi, &line, &eof)) != OK){
			return st;
		}
		if((st = split(line, &fields, delimiter)) != OK){
			return st;
		}
		if((st = ADT_NMEA_GGA_new(&node, fields)) != OK){
			for(i = 0; fields[i] != NULL; i++)
				free(fields[i]);
			free(fields);
			return st;
		}


		for(i = 0; fields[i] != NULL; i++)
			free(fields[i]);
		free(fields);
	}

	return OK;
}

status_t parse_file_format(const string *format_string, file_format_t *file_format)
{
	if(!strcmp(format_string, FILE_FORMAT_KML)){
		*file_format = FORMAT_KML;
		return OK;
	}
	if(!strcmp(format_string, FILE_FORMAT_CSV)){
		*file_format = FORMAT_CSV;
		return OK;
	}

	return ERROR_UNKNOWN_FILE_FORMAT;
}