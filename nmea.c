#include <string.h>

#include "nmea"

status_t parse_NMEA()
{

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
	
	return OK;
}