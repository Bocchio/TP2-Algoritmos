#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "nmea.h"

status_t parse_NMEA_from_csv(FILE *fi, ADT_Vector_t **gga_vector, string delimiter)
{
	status_t st;
	size_t i;
	bool_t eof = FALSE;
	string line;
	string *fields;
	ADT_NMEA_GGA_t *node;
	functions_interface_t gga_functions;

	gga_functions.destructor = &ADT_NMEA_GGA_delete;
	gga_functions.clonator = NULL;

	if((st = ADT_Vector_new(gga_vector, gga_functions)) != OK){
		return st;
	}

	/* Read every line of the file */
	while(eof == FALSE){
		/* Get the fields*/
		if((st = readline(fi, &line, &eof)) != OK){
			return st;
		}
		if((st = split(line, &fields, delimiter)) != OK){
			return st;
		}
		free(line);

		/* If it's a GGA node append it to the vector */
		if(!strcmp(fields[0], GPGGA_HEADER)){
			if((st = ADT_NMEA_GGA_new(&node, fields)) != OK){
				free_string_array(fields);
				return st;
			}
			if((st = ADT_Vector_append(*gga_vector, node)) != OK){
				free_string_array(fields);
				return st;
			}
		}
		free_string_array(fields);
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

status_t ADT_NMEA_GGA_export_as_kml(ADT_NMEA_GGA_t *gga, void *tabs, FILE *fo)
{
	size_t i;

	if(fo == NULL || tabs == NULL || gga == NULL)
		return ERROR_NULL_POINTER;
	
	/* Tabulate */
	for(i = 0; i < *(size_t *)tabs; i++){
		if(fputc('\t', fo) == EOF)
			return ERROR_WRITING_FILE;
	}
	/* Print the data */
	if(fprintf(fo, "%f%d%f%c%f", gga->latitude, ',', gga->longitude, ',', gga->altitude) < 0)
		return st;

	return OK;
}