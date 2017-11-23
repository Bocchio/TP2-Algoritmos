#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "utils.h"

status_t ADT_Vector_new(ADT_Vector_t **vector)
{
	if(vector == NULL)
		return ERROR_NULL_POINTER;

	if((*vector = (ADT_Vector_t *) malloc(sizeof(ADT_Vector_t))) == NULL)
		return ERROR_MEMORY;
	if((*vector)->elements=(void **)malloc(sizeof(void *)*INIT_CHOP))==NULL){
		free(*vector);
		*vector=NULL;
		return ERROR_MEMORY;
	}
	(*vector)->alloc_size = INIT_CHOP;
	(*vector)->elements = NULL;
	(*vector)->len = 0;
	(*vector)->label = "";
	(*vector)->delete_element = NULL;
	(*vector)->clone_element = NULL;
	(*vector)->export_element_as_csv = NULL;
	(*vector)->export_element_as_kml = NULL;

	return OK;
}

status_t ADT_Vector_delete(ADT_Vector_t **vector)
{
	size_t i;
	status_t st;

	if(vector == NULL)
		return ERROR_NULL_POINTER;

	for(i = 0; i < (*vector)->len; i++){
		if((st = (*vector)->delete_element((*vector)->elements + i)) != OK)
			return st;
	}
	
	free((*vector)->elements);
	free(*vector);
	*vector = NULL;

	return OK;
}

status_t ADT_Vector_append(ADT_Vector_t *vector, void *new_item)
{
	size_t i;
	void **aux;
	size_t new_alloc_size;

	if(vector == NULL || new_item == NULL)
		return ERROR_NULL_POINTER;

	/* Allocate more space if we need to */
	if(vector->len == vector->alloc_size){
		new_alloc_size = vector->alloc_size + ADT_Vector_CHOP_SIZE;
		if((aux = (void **) realloc(vector->elements, new_alloc_size*sizeof(void *))) == NULL)
			return ERROR_MEMORY;
		vector->elements = aux;
		vector->alloc_size = new_alloc_size;
		
		/* Set every non-used position to NULL */
		for(i = vector->len; i < vector->alloc_size; i++){
			vector->elements[i] = NULL;
		}
	}

	vector->elements[vector->len] = new_item;
	vector->len++;

	return OK;
}

void * ADT_Vector_get_element(const ADT_Vector_t *vector, size_t pos)
{
	if(vector == NULL)
		return NULL;

	if(pos >= vector->len)
		return NULL;

	return vector->elements[pos];
}

status_t ADT_Vector_export_as_csv(const ADT_Vector_t *vector, void *ctx, FILE *fo)
{
	size_t i;
	status_t st;

	for(i=0;i<vector->len;i++){
		if((st = vector->export_element_as_csv(vector->elements[i], ctx, fo)) != OK)
			return st;
	}

	return OK;
}

status_t ADT_Vector_export_as_kml(const ADT_Vector_t *vector, void *_ctx, FILE *fo)
{
	size_t i;
	status_t st;
	xml_ctx_t *ctx;
	xml_ctx_t element_ctx;

	if(vector == NULL || _ctx == NULL || fo == NULL)
		return ERROR_NULL_POINTER;

	ctx = (xml_ctx_t *) _ctx;

	/* The kml context that each element of the vector will use */
	element_ctx.header = "";
	element_ctx.footer = "";
	element_ctx.indentation = ctx->indentation + 1;

	if(fputs(ctx->header, fo) == EOF)
		return ERROR_WRITING_FILE;

	/* open the vectors tag */
	if(fprintf("%c%s%c\n", '<', vector->label, '>') < 0)
		return ERROR_WRITING_FILE;

	/* Export each element */
	for(i = 0; i < vector->len; i++){
		if((st = vector->export_element_as_kml(vector->elements[i], &element_ctx, fo)) != OK)
			return st;
	}

	/* closes the vectors tag */
	if(fprintf("%s%s%c\n", "</", vector->label, '>') < 0)
		return ERROR_WRITING_FILE;

	if(fputs(ctx->footer, fo) == EOF)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t set_destructor(ADT_Vector_t *vector, destructor_t destructor)
{
	if(vector == NULL)
		return ERROR_NULL_POINTER;

	vector->delete_element = destructor;

	return OK;
}

status_t set_clonator(ADT_Vector_t *vector, clonator_t clonator)
{
	if(vector == NULL)
		return ERROR_NULL_POINTER;

	vector->clone_element = clonator;

	return OK;
}

status_t set_csv_exporter(ADT_Vector_t *vector, printer_t csv_exporter)
{
	if(vector == NULL)
		return ERROR_NULL_POINTER;

	vector->export_element_as_csv = csv_exporter;

	return OK;
}

status_t set_kml_exporter(ADT_Vector_t *vector, printer_t kml_exporter)
{
	if(vector == NULL)
		return ERROR_NULL_POINTER;

	vector->export_element_as_kml = kml_exporter;

	return OK;
}
