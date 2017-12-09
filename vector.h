#ifndef VECTOR__H
#define VECTOR__H

#include <stdio.h>
#include "types.h"

#define ADT_Vector_INIT_CHOP    15
#define ADT_Vector_CHOP_SIZE    20

typedef struct {
    void **elements;
    size_t len;
    size_t alloc_size;
    string tag_name;
    destructor_t destroy_element;
    clonator_t clone_element;
    printer_t export_element_as_csv;
    printer_t export_element_as_kml;
} ADT_Vector_t;

/* Constructors */
status_t ADT_Vector_new(ADT_Vector_t **vector);

/* Destructor */
status_t ADT_Vector_destroy(ADT_Vector_t **vector);

status_t ADT_Vector_append(ADT_Vector_t *vector, void *new_item);
void * ADT_Vector_get_element(const ADT_Vector_t *vector, size_t pos);

status_t ADT_Vector_export_as_csv(const ADT_Vector_t *vector, const string delim, FILE *fo);
status_t ADT_Vector_export_as_kml(const ADT_Vector_t *vector, const kml_context_t *context, FILE *fo);

/* Setters */
status_t ADT_Vector_set_destructor(ADT_Vector_t *vector, destructor_t destructor);
status_t ADT_Vector_set_clonator(ADT_Vector_t *vector, clonator_t clonator);
status_t ADT_Vector_set_csv_exporter(ADT_Vector_t *vector, printer_t csv_exporter);
status_t ADT_Vector_set_kml_exporter(ADT_Vector_t *vector, printer_t kml_exporter);
status_t ADT_Vector_set_tag_name(ADT_Vector_t *vector, string tag);

#endif
