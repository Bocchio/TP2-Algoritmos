#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "utils.h"
#include "xml.h"

status_t ADT_Vector_new(ADT_Vector_t **vector, functions_interface_t *element_functions)
{
	if(vector == NULL)
		return ERROR_NULL_POINTER;

	if((*vector = (ADT_Vector_t *) malloc(sizeof(ADT_Vector_t))) == NULL)
		return ERROR_MEMORY;

	(*vector)->elements = NULL;
	(*vector)->len = 0;
	(*vector)->alloc_size = 0;
	(*vector)->element_label = NULL;

	(*vector)->xml_header = NULL;
	(*vector)->xml_footer = NULL;

	(*vector)->delete_element = element_functions->destructor;
	(*vector)->clone_element = element_functions->clonator;
	(*vector)->export_element_as_csv = element_functions->csv_exporter;
	(*vector)->export_element_as_xml = element_functions->xml_exporter;

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

status_t ADT_Vector_export_as_csv(ADT_Vector_t *vector, void *ctx, FILE *fo)
{
	size_t i;
	status_t st;

	for(i=0;i<vector->len;i++){
		if((st = vector->export_element_as_csv(vector->elements[i], ctx, fo)) != OK)
			return st;
	}

	return OK;
}

status_t ADT_Vector_export_as_xml(const ADT_Vector_t *vector, void *_ctx, FILE *fo)
{
	size_t i, j;
	status_t st;
	xml_ctx_t *ctx;
	xml_ctx_t element_ctx;

	if(vector == NULL || xml_ctx == NULL || fo == NULL)
		return ERROR_NULL_POINTER;

	ctx = (xml_ctx_t *) _ctx;

	/* The xml context that each element of the vector will use */
	element_ctx->print_header = FALSE;
	element_ctx->previous_chunk = NULL;
	element_ctx->next_chunk = NULL;
	element_ctx->attributes = NULL;
	element_ctx->label = NULL;
	element_ctx->indentation = ctx->indentation + 1;

	if(ctx->print_header == TRUE){
		if((st = xml_print_header(fo)) != OK)
			return st;
	}

	if(cxt->previous_chunk != NULL){
		if(fputs(vector->xml_before_chunk, fo) == EOF)
			return ERROR_WRITING_FILE;
	}

	/* If the label is not NULL nor "" */
	if((ctx->label != NULL) && *(ctx->label)){
		if((st = xml_open_tag(ctx->label, ctx->attributes, ctx->indentation, fo)) != OK)
			return st;
	}

	/* Export each element */
	for(i = 0; i < vector->len; i++){
		if((st = vector->export_element_as_xml(vector->elements[i], &element_ctx, fo)) != OK)
			return st;
	}

	/* Same as before */
	if((ctx->label != NULL) && *(ctx->label)){
		if((st = xml_close_tag(ctx->label, ctx->indentation, fo)) != OK)
			return st;
	}

	if(ctx->next_chunk != NULL){
		if(fputs(ctx->next_chunk, fo) == EOF)
			return ERROR_WRITING_FILE;
	}

	return OK;
}