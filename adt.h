#ifndef ADT__H
#define ADT__H

#include "types.h"

typedef status_t (*destructor_t)(void **);
typedef void * (*clonator_t)(const void *);
typedef status_t (*printer_t)(const void *, void *ctx, FILE *fo);

typedef struct {
	destructor_t destructor;
	clonator_t clonator;
	printer_t csv_exporter;
	printer_t xml_exporter;
} functions_interface_t;

#endif