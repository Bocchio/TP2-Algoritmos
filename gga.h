#ifndef GGA__H
#define GGA__H

#include "types.h"
#include "adt.h"

typedef struct{
	double latitude;
	double longitude;
	double altitude; 
	functions_interface_t functions_interface; 
}ADT_GGA_t;


status_t ADT_GGA_new(ADT_Vector_t **vector, functions_interface_t *element_functions);
status_t ADT_GGA_delete(ADT_Vector_t **vector);

#endif
