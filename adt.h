#ifndef ADT__H
#define ADT__H

typedef status_t (*destructor_t)(void **);
typedef void * (*clonator_t)(const void *);

typedef struct {
	destructor_t destructor;
	clonator_t clonator;
} functions_interface_t;

#endif