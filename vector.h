#ifndef VECTOR__H
#define VECTOR__H

#include "types.h"
#include "adt.h"

#define ADT_Vector_CHOP_SIZE	20

typedef struct {
	void **elements;
	size_t len;
	size_t alloc_size;
	destructor_t delete_element;
	clonator_t clone_element;
	printer_t export_element_as_csv;
	printer_t export_element_as_xml;
} ADT_Vector_t;

status_t ADT_Vector_new(ADT_Vector_t **vector, functions_interface_t *element_functions);
status_t ADT_Vector_delete(ADT_Vector_t **vector);
status_t ADT_Vector_append(ADT_Vector_t *vector, void *new_item);
status_t ADT_Vector_export_as_csv(const ADT_Vector_t *vector, void *ctx, FILE *output_file);
status_t ADT_Vector_export_as_xml(const ADT_Vector_t *vector, void *tabs, FILE *output_file);
void * ADT_Vector_get_element(const ADT_Vector_t *vector, size_t pos);

#endif