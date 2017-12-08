#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "vector.h"
#include "nmea.h"
#include "types.h"

status_t ADT_NMEA_record_load_fields(FILE *fi, ADT_Vector_t **gga_vector)
{
	status_t st;
	bool_t eof = FALSE;
	string line;
	string *fields;
	size_t len_fields_array;
	ADT_NMEA_GGA_t *node;

	if((st = ADT_Vector_new(gga_vector)) != OK){
		return st;
	}

	if((st = ADT_Vector_set_csv_exporter(*gga_vector, ADT_NMEA_record_export_as_csv)) != OK){
		ADT_NMEA_record_delete_fields(gga_vector);
		return st;
	}

	if((st = ADT_Vector_set_kml_exporter(*gga_vector, ADT_NMEA_record_export_as_kml)) != OK){
		ADT_NMEA_record_delete_fields(gga_vector);
		return st;
	}

	if((st = ADT_Vector_set_label(*gga_vector, KML_COORDINATES_TAG)) != OK){
		ADT_NMEA_record_delete_fields(gga_vector);
		return st;
	}

	/* Read every line of the file */
	while(eof == FALSE){
		if((st = readline(fi, &line, &eof)) != OK){
			return st;
		}
		/* Create an array of fields from the line read */
		if((st = split(line, &fields, NMEA_FIELD_DELIMITER, &len_fields_array)) != OK){
			return st;
		}
		free(line);

		/* If it's a GGA message */
		if(!strcmp(fields[GPGGA_HEADER_FIELD_INDEX], GPGGA_HEADER)){
			/* create the GGA node */
			if((st = ADT_NMEA_record_new(&node, fields)) != OK){
				free_string_array(&fields, len_fields_array);
				ADT_NMEA_record_delete_fields(gga_vector);
				return st;
			}
			/* If the node is not NULL (i.e, it contained geografic information) */
			if(node != NULL){
				/* then append it into vector */
				if((st = ADT_Vector_append(*gga_vector, node)) != OK){
					free_string_array(&fields, len_fields_array);
					ADT_NMEA_record_delete_fields(gga_vector);
					return st;
				}
			}
		}
		free_string_array(&fields, len_fields_array);
	}

	return OK;
}

status_t ADT_NMEA_record_fields_new_from_strings(ADT_GGA_t **gga_node, string *fields)
{
	status_t st;
	bool_t is_empty;
	char *tmp;

	if((*gga_node = (ADT_NMEA_GGA_t *) malloc(sizeof(ADT_NMEA_GGA_t))) == NULL)
		return ERROR_MEMORY;

	if((st = ADT_NMEA_record_parse_latitude(fields[GPGGA_LON_FIELD_INDEX], &((*gga_node)->longitude), &is_empty)) != OK){
		ADT_NMEA_record_delete_fields(gga_node);
		return st;
	}

	if((st = ADT_NMEA_record_parse_longitude(fields[GPGGA_LAT_FIELD_INDEX], &((*gga_node)->latitude), &is_empty)) != OK){
		ADT_NMEA_record_delete_fields(gga_node);
		return st;
	}

	if(is_empty == TRUE){
		ADT_NMEA_record_delete_fields(gga_node);
		return OK;
	}

	(*gga_node)->altitude = strtod(fields[GPGGA_ALT_FIELD_INDEX], &tmp);
	if(*tmp){
		ADT_NMEA_record_delete_fields(gga_node);
		return ERROR_READING_FILE;
	}

	if(!strcmp(fields[GPGGA_NS_INDICATOR_POS], GPGGA_SOUTH_TOKEN)){
		(*gga_node)->latitude *= -1;
	}
	else if(strcmp(fields[GPGGA_NS_INDICATOR_POS], GPGGA_NORTH_TOKEN)){
		ADT_NMEA_record_delete_fields(gga_node);
		return ERROR_READING_FILE;
	}

	if(!strcmp(fields[GPGGA_EW_INDICATOR_POS], GPGGA_WEST_TOKEN)){
		(*gga_node)->longitude *= -1;
	}
	else if(strcmp(fields[GPGGA_EW_INDICATOR_POS], GPGGA_EAST_TOKEN)){
		ADT_NMEA_record_delete_fields(gga_node);
		return ERROR_READING_FILE;
	}

	return OK;

}

status_t ADT_NMEA_record_delete_fields(ADT_GGA_t **gga_node)
{
	free(*gga_node);
	*gga_node = NULL;

	return OK;
}

status_t ADT_NMEA_record_export_as_kml(const ADT_GGA_t *gga, void *_ctx, FILE *fo)
{
	uchar i;
	xml_context_t *context;

	if(gga == NULL || _ctx == NULL || fo == NULL)
		return ERROR_NULL_POINTER;

	ctx = (xml_context_t *) _ctx;

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

status_t ADT_NMEA_record_export_as_csv(const ADT_GGA_t *gga, void *ctx, FILE *fo)
{
	string delim;

	if(gga == NULL || ctx == NULL || fo == NULL)
		return ERROR_NULL_POINTER;
	
	delim = ctx;
	if(fprintf(fo, "%f%s%f%s%i\n", gga->longitude, delim, gga->latitude, delim, (int) gga->altitude) < 0)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t NMEA_export_as_csv(const ADT_Vector_t *vector, void *context, FILE *fo)
{
	status_t st;
	*context = OUTPUT_CSV_DELIMITER;

	if(vector == NULL)
		return ERROR_NULL_POINTER;

	if((st = ADT_Vector_export_as_csv(vector, context, fo)) != OK)
		return st;

	NMEA_destroy_csv_ctx(&context);

	return OK;
}

status_t NMEA_export_as_kml(const ADT_Vector_t *vector, void * context, FILE *fo)
{
	status_t st;
	
	if(vector == NULL)
		return ERROR_NULL_POINTER;

	if((st = NMEA_get_kml_ctx(&context)) != OK)
		return st;

	if((st = ADT_Vector_export_as_kml(vector, context, fo)) != OK)
		return st;

	NMEA_destroy_kml_ctx(&context);

	return OK;
}

status_t NMEA_get_kml_ctx(void **ctx)
{
	status_t st;
	FILE *header_file;
	FILE *footer_file;
	xml_context_t *kml_ctx;
	
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
	xml_context_t **ctx = (xml_context_t **) _ctx;

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
