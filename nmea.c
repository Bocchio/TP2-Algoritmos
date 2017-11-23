#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "vector.h"
#include "nmea.h"

status_t parse_NMEA(FILE *fi, ADT_Vector_t **gga_vector)
{
	status_t st;
	bool_t eof = FALSE;
	string line;
	string *fields;
	ADT_NMEA_GGA_t *node;

	if((st = ADT_Vector_new(gga_vector)) != OK){
		return st;
	}

	if((st = ADT_Vector_set_csv_exporter(*gga_vector, (printer_t) ADT_NMEA_GGA_export_as_csv)) != OK){
		ADT_Vector_delete(gga_vector);
		return st;
	}

	if((st = ADT_Vector_set_kml_exporter(*gga_vector, (printer_t) ADT_NMEA_GGA_export_as_kml)) != OK){
		ADT_Vector_delete(gga_vector);
		return st;
	}

	if((st = ADT_Vector_set_label(*gga_vector, KML_COORDINATES_LABEL)) != OK){
		ADT_Vector_delete(gga_vector);
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
				ADT_Vector_delete(gga_vector);
				return st;
			}
			if(node != NULL){
				if((st = ADT_Vector_append(*gga_vector, node)) != OK){
					free_string_array(&fields);
					ADT_Vector_delete(gga_vector);
					return st;
				}
			}
		}
		free_string_array(&fields);
	}

	return OK;
}

status_t ADT_NMEA_GGA_new(ADT_NMEA_GGA_t **gga_node, string *fields)
{
	status_t st;
	bool_t is_empty;
	char *tmp;

	if((*gga_node = (ADT_NMEA_GGA_t *) malloc(sizeof(ADT_NMEA_GGA_t))) == NULL)
		return ERROR_MEMORY;

	if((st = parse_NMEA_latitude(fields[GPGGA_LON_POS], &((*gga_node)->longitude), &is_empty)) != OK){
		ADT_NMEA_GGA_delete(gga_node);
		return st;
	}

	if((st = parse_NMEA_longitude(fields[GPGGA_LAT_POS], &((*gga_node)->latitude), &is_empty)) != OK){
		ADT_NMEA_GGA_delete(gga_node);
		return st;
	}

	if(is_empty == TRUE){
		ADT_NMEA_GGA_delete(gga_node);
		return OK;
	}

	(*gga_node)->altitude = strtod(fields[GPGGA_ALT_POS], &tmp);
	if(*tmp){
		ADT_NMEA_GGA_delete(gga_node);
		return ERROR_READING_FILE;
	}

	if(!strcmp(fields[GPGGA_NS_INDICATOR_POS], GPGGA_SOUTH_TOKEN)){
		(*gga_node)->latitude *= -1;
	}
	else if(strcmp(fields[GPGGA_NS_INDICATOR_POS], GPGGA_NORTH_TOKEN)){
		ADT_NMEA_GGA_delete(gga_node);
		return ERROR_READING_FILE;
	}

	if(!strcmp(fields[GPGGA_EW_INDICATOR_POS], GPGGA_WEST_TOKEN)){
		(*gga_node)->longitude *= -1;
	}
	else if(strcmp(fields[GPGGA_EW_INDICATOR_POS], GPGGA_EAST_TOKEN)){
		ADT_NMEA_GGA_delete(gga_node);
		return ERROR_READING_FILE;
	}

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
	uchar i;
	xml_ctx_t *ctx;

	if(gga == NULL || _ctx == NULL || fo == NULL)
		return ERROR_NULL_POINTER;

	ctx = (xml_ctx_t *) _ctx;

	/* Tabulate the line*/
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
	/* Context for each export function */
	creator_t ctx_getters[] = {
		(creator_t) NMEA_get_csv_ctx,
		(creator_t) NMEA_get_kml_ctx
	};
	/* Context destructors for each format */ 
	destructor_t ctx_destructors[] = {
		(destructor_t) NMEA_destroy_csv_ctx,
		(destructor_t) NMEA_destroy_kml_ctx
	};
	/* Dispatch table for each format */
	printer_t export_lookup_table[] = {
		(printer_t) &ADT_Vector_export_as_csv,
		(printer_t) &ADT_Vector_export_as_kml
	};

	if(vector == NULL)
		return ERROR_NULL_POINTER;

	if((st = ctx_getters[format](&ctx)) != OK)
		return st;

	if((st = export_lookup_table[format](vector, ctx, fo)) != OK)
		return st;

	ctx_destructors[format](&ctx);

	return OK;
}

status_t NMEA_get_csv_ctx(void **ctx)
{
	if(ctx == NULL)
		return ERROR_NULL_POINTER;

	*ctx = OUTPUT_CSV_DELIMITER;

	return OK;
}

status_t NMEA_get_kml_ctx(void **ctx)
{
	status_t st;
	FILE *header_file;
	FILE *footer_file;
	xml_ctx_t *kml_ctx;
	
	if(ctx == NULL)
		return ERROR_NULL_POINTER;

	if((kml_ctx = (xml_ctx_t *) malloc(sizeof(xml_ctx_t))) == NULL)
		return ERROR_MEMORY;

	if((header_file = fopen(KML_HEADER_FILE_PATH, "rt")) == NULL){
		NMEA_destroy_kml_ctx((void **) &kml_ctx);
		return ERROR_OPENING_FILE;
	}

	if((footer_file = fopen(KML_FOOTER_FILE_PATH, "rt")) == NULL){
		NMEA_destroy_kml_ctx((void **) &kml_ctx);
		return ERROR_OPENING_FILE;
	}

	/* get the header */
	if((st = readfile(header_file, &(kml_ctx->header))) != OK){
		NMEA_destroy_kml_ctx((void **) &kml_ctx);
		return st;
	}

	/* get the footer */
	if((st = readfile(footer_file, &(kml_ctx->footer))) != OK){
		NMEA_destroy_kml_ctx((void **) &kml_ctx);
		return st;
	}

	fclose(header_file);
	fclose(footer_file);

	kml_ctx->indentation = OUTPUT_KML_INDENTATION;

	*ctx = kml_ctx;
	
	return OK;
}

status_t NMEA_destroy_csv_ctx(void **_ctx)
{
	*_ctx = NULL; 

	return OK;
}

status_t NMEA_destroy_kml_ctx(void **_ctx)
{
	xml_ctx_t **ctx = (xml_ctx_t **) _ctx;

	free((*ctx)->header);
	free((*ctx)->footer);
	free((*ctx));
	*ctx = NULL;

	return OK;
}

status_t parse_NMEA_latitude(string coord, double *degrees, bool_t *is_empty)
{
	char *tmp;
	char *minutes_pos;

	*degrees = 0;

	if((minutes_pos = strchr(coord, '.')) == NULL){
		/* if coord isn't "" then the file is corrupt*/
		if(*coord)
			return ERROR_READING_FILE;
		*is_empty = TRUE;
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

	*is_empty = FALSE;

	return OK;
}

status_t parse_NMEA_longitude(string coord, double *degrees, bool_t *is_empty)
{
	char *tmp;
	char *minutes_pos;

	*degrees = 0;

	if((minutes_pos = strchr(coord, '.')) == NULL){
		/* if coord isn't "" then the file is corrupt*/
		if(*coord)
			return ERROR_READING_FILE;
		*is_empty = TRUE;
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

	*is_empty = FALSE;

	return OK;
}
