#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

status_t ADT_Vector_new(ADT_Vector_t **vector, functions_interface_t *element_functions)
{
	if(vector == NULL)
		return ERROR_NULL_POINTER;

	if((*vector = (ADT_Vector_t *) malloc(sizeof(ADT_Vector_t))) == NULL)
		return ERROR_MEMORY;

	(*vector)->elements = NULL;
	(*vector)->len = 0;
	(*vector)->alloc_size = 0;
	(*vector)->delete_element = element_functions->destructor;
	(*vector)->clone_element = element_functions->clonator;

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

status_t ADT_Vector_export_as_csv(ADT_Vector_t *vector,FILE*output_file,*void del);