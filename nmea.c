#include <string.h>
#include <stdio.h>
#include "nmea"

status_t parse_NMEA(FILE *fi, ADT_Vector_t *gga)
{
	status_t st;
	bool_t eof = FALSE;
	string line;
	string *fields;

	while(eof == FALSE){
		if((st = readline(fi, &line, &eof)) != OK){
			return st;
		}
		if((st = ) != OK){

		}

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