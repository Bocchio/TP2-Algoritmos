#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "nmea.h"
#include "adt.h"

/* kml context for the export functions */
xml_ctx_t kml_ctx = {
	TRUE, /* print the xml header */
	KML_HEADER, /* open the kml file */
	KML_FOOTER, /* close the kml file */
	NULL, /* no attributes */
	NMEA_GGA_LABEL, /* label of the GGA information */
	4 /* tabs of indentation */
};

/* Context for the export functions, corresponds with file_format_t */
void * context_lookup_table = {
	OUTPUT_CSV_DELIMITER,
	&kml_ctx
};

/* Dispatch table that corresponds with file_format_t */
printer_t export_lookup_table[] = {
	&ADT_Vector_export_as_csv,
	&ADT_Vector_export_as_xml
};


status_t parse_NMEA_from_csv(FILE *fi, ADT_Vector_t **gga_vector)
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
		if((st = split(line, &fields, NMEA_CSV_DELIMITER)) != OK){
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

status_t ADT_NMEA_GGA_export_as_kml(ADT_NMEA_GGA_t *gga, void *xml_ctx, FILE *fo)
{
	size_t i;
	xml_ctx_t *ctx;

	if(gga == NULL || ctx == NULL || fo == NULL)
		return ERROR_NULL_POINTER;
	
	ctx = (xml_ctx_t *) xml_ctx;

	/* Tabulate */
	for(i = 0; i < *(uchar *)tabs; i++){
		if(fputc('\t', fo) == EOF)
			return ERROR_WRITING_FILE;
	}
	/* Print the data */
	if(fprintf(fo, "%f%c%f%c%f", gga->latitude, ',', gga->longitude, ',', gga->altitude) < 0)
		return st;

	return OK;
}

status_t ADT_NMEA_GGA_export_as_csv(ADT_NMEA_GGA_t *gga, void *ctx, FILE *fo)
{
	size_t i;
	string delim;

	if(gga == NULL || ctx == NULL || fo == NULL)
		return ERROR_NULL_POINTER;
	
	delim = ctx;
	if(fprintf(fo, "%f%s%f%s%f", gga->latitude, delim, gga->longitude, delim, gga->altitude) < 0)
		return st;

	return OK;
}

status_t export_NMEA(const ADT_Vector_t *vector, file_format_t format, FILE *fo)
{
	void *ctx;

	if(vector == NULL)
		return ERROR_NULL_POINTER;

	ctx = context_lookup_table[format];
	if((st = export_lookup_table[format](vector, ctx, fo)) != OK)
		return st;

	return OK;
}