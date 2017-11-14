#ifndef ADT__H
#define ADT__H

#include "types.h"

typedef status_t (*destructor_t)(void **);
typedef void * (*clonator_t)(const void *);
typedef status_t (*exporter_t)(const void *, void *ctx, FILE *fo);

typedef struct {
	destructor_t destructor;
	clonator_t clonator;
	exporter_t csv_exporter;
	exporter_t kml_exporter;
} functions_interface_t;

#endif