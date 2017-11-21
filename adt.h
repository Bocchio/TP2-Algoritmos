#ifndef ADT__H
#define ADT__H

#include "types.h"

typedef status_t (*destructor_t)(void **);
typedef void * (*clonator_t)(const void *);
typedef status_t (*printer_t)(const void *, void *ctx, FILE *fo);

#endif
