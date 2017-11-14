#ifndef TYPES__H
#define TYPES__H

typedef enum {
	OK,
	ERROR_NULL_POINTER,
	ERROR_MEMORY
} status_t;

typedef enum {
	FALSE,
	TRUE
} bool_t;

typedef char * string;

#endif 
