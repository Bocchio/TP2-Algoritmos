#ifndef VECTOR__H
#define VECTOR__H

#include "types.h"

#define ADT_Vector_CHOP_SIZE	20

typedef struct {
	void **elements;
	size_t len;
	size_t alloc_size;
	string label;
	destructor_t delete_element;
	clonator_t clone_element;
	printer_t export_element_as_csv;
	printer_t export_element_as_kml;
} ADT_Vector_t;

status_t ADT_Vector_new(ADT_Vector_t **vector);
status_t ADT_Vector_delete(ADT_Vector_t **vector);
status_t ADT_Vector_append(ADT_Vector_t *vector, void *new_item);
status_t ADT_Vector_export_as_csv(const ADT_Vector_t *vector, void *ctx, FILE *fo);
status_t ADT_Vector_export_as_kml(const ADT_Vector_t *vector, void *ctx, FILE *fo);
void * ADT_Vector_get_element(const ADT_Vector_t *vector, size_t pos);

status_t set_destructor(ADT_Vector_t *vector, destructor_t destructor);
status_t set_clonator(ADT_Vector_t *vector, clonator_t clonator);
status_t set_csv_exporter(ADT_Vector_t *vector, printer_t csv_exporter);
status_t set_kml_exporter(ADT_Vector_t *vector, printer_t kml_exporter);

#endif
