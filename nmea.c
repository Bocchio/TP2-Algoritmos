#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "vector.h"
#include "nmea.h"
#include "adt.h"
#include "xml.h"

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
void * context_lookup_table[] = {
	OUTPUT_CSV_DELIMITER,
	&kml_ctx
};

/* Dispatch table that corresponds with file_format_t */
printer_t export_lookup_table[] = {
	(printer_t) &ADT_Vector_export_as_csv,
	(printer_t) &ADT_Vector_export_as_xml
};


status_t parse_NMEA(FILE *fi, ADT_Vector_t **gga_vector)
{
	status_t st;
	bool_t eof = FALSE;
	string line;
	string *fields;
	ADT_NMEA_GGA_t *node;
	functions_interface_t gga_functions;

	gga_functions.destructor = (destructor_t) &ADT_NMEA_GGA_delete;
	gga_functions.clonator = (clonator_t) NULL;
	gga_functions.csv_exporter = (printer_t) &ADT_NMEA_GGA_export_as_csv;
	gga_functions.xml_exporter = (printer_t) &ADT_NMEA_GGA_export_as_kml;

	if((st = ADT_Vector_new(gga_vector, &gga_functions)) != OK){
		return st;
	}

	/* Read every line of the file */
	while(eof == FALSE){
		/* Get the fields*/
		if((st = readline(fi, &line, &eof)) != OK){
			return st;
		}
		if((st = split(line, &fields, NMEA_FIELD_DELIMITER)) != OK){
			return st;
		}
		free(line);

		/* If it's a GGA node append it to the vector */
		if(!strcmp(fields[0], GPGGA_HEADER)){
			if((st = ADT_NMEA_GGA_new(&node, fields)) != OK){
				free_string_array(&fields);
				return st;
			}
			if((st = ADT_Vector_append(*gga_vector, node)) != OK){
				free_string_array(&fields);
				return st;
			}
		}
		free_string_array(&fields);
	}

	return OK;
}

status_t ADT_NMEA_GGA_new(ADT_NMEA_GGA_t **gga_node, string *fields)
{
	status_t st;
	char *tmp;

	if((*gga_node = (ADT_NMEA_GGA_t *) malloc(sizeof(ADT_NMEA_GGA_t))) == NULL)
		return ERROR_MEMORY;

	if((st = parse_NMEA_latlon(fields[GPGGA_LONGITUDE_POS], &((*gga_node)->longitude))) != OK){
		ADT_NMEA_GGA_delete(gga_node);
		return st;
	}

	if((st = parse_NMEA_latlon(fields[GPGGA_LATITUDE_POS], &((*gga_node)->latitude))) != OK){
		ADT_NMEA_GGA_delete(gga_node);
		return st;
	}

	(*gga_node)->altitude = strtod(fields[GPGGA_ALTITUDE_POS], &tmp);
	if(*tmp){
		ADT_NMEA_GGA_delete(gga_node);
		return ERROR_READING_FILE;
	}

	if(*fields[GPGGA_LATITUDE_POS]){
		(*gga_node)->has_coord = TRUE;
		if(!strcmp(fields[GPGGA_NS_INDICATOR_POS], GPGGA_SOUTH_TOKEN)){
			(*gga_node)->latitude *= -1;
		}
		else if(strcmp(fields[GPGGA_NS_INDICATOR_POS], GPGGA_NORTH_TOKEN)){
			ADT_NMEA_GGA_delete(gga_node);
			return ERROR_READING_FILE;
		}
	}
	else
		(*gga_node)->has_coord = FALSE;

	if(*fields[GPGGA_LONGITUDE_POS]){
		(*gga_node)->has_coord = TRUE;
		if(!strcmp(fields[GPGGA_EW_INDICATOR_POS], GPGGA_WEST_TOKEN)){
			(*gga_node)->longitude *= -1;
		}
		else if(strcmp(fields[GPGGA_EW_INDICATOR_POS], GPGGA_EAST_TOKEN)){
			ADT_NMEA_GGA_delete(gga_node);
			return ERROR_READING_FILE;
		}
	}
	else
		(*gga_node)->has_coord = FALSE;

	return OK;

}

status_t ADT_NMEA_GGA_delete(ADT_NMEA_GGA_t **gga_node)
{
	free(*gga_node);
	gga_node = NULL;

	return OK;
}

status_t ADT_NMEA_GGA_export_as_kml(const ADT_NMEA_GGA_t *gga, void *_ctx, FILE *fo)
{
	size_t i;
	xml_ctx_t *ctx;

	if(gga == NULL || _ctx == NULL || fo == NULL)
		return ERROR_NULL_POINTER;
	
	if(gga->has_coord == FALSE)
		return OK;

	ctx = (xml_ctx_t *) _ctx;

	/* Tabulate */
	for(i = 0; i < ctx->indentation; i++){
		if(fputc('\t', fo) == EOF)
			return ERROR_WRITING_FILE;
	}
	/* Print the data */
	if(fprintf(fo, "%f%c%f%c%i\n", gga->longitude, ',', gga->latitude, ',', (int) gga->altitude) < 0)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t ADT_NMEA_GGA_export_as_csv(const ADT_NMEA_GGA_t *gga, void *ctx, FILE *fo)
{
	string delim;

	if(gga == NULL || ctx == NULL || fo == NULL)
		return ERROR_NULL_POINTER;
	
	delim = ctx;
	if(fprintf(fo, "%f%s%f%s%i\n", gga->longitude, delim, gga->latitude, delim, (int) gga->altitude) < 0)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t export_NMEA(const ADT_Vector_t *vector, file_format_t format, FILE *fo)
{
	status_t st;
	void *ctx;

	if(vector == NULL)
		return ERROR_NULL_POINTER;

	ctx = context_lookup_table[format];
	if((st = export_lookup_table[format](vector, ctx, fo)) != OK)
		return st;

	return OK;
}


status_t parse_NMEA_latlon(string coord, double *degrees)
{
	char *tmp;
	char *minutes_pos;

	*degrees = 0;

	if((minutes_pos = strchr(coord, '.')) == NULL){
		/* if coord is not "" then the file is corrupt*/
		if(*coord)
			return ERROR_READING_FILE;
		else
			return OK;
	}
	minutes_pos -= 2;
	/* 60 minutes in a degree */
	*degrees += strtod(minutes_pos, &tmp)/60.0;
	if(*tmp)
		return ERROR_READING_FILE;

	*minutes_pos = '\0';
	*degrees += strtod(coord, &tmp);
	if(*tmp)
		return ERROR_READING_FILE;

	return OK;
}