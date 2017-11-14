#ifndef TYPES__H
#define TYPES__H

typedef enum {
	OK,
	ERROR_NULL_POINTER,
	ERROR_MEMORY,
	ERROR_OPENING_FILE,
	ERROR_UNKNOWN_FILE_FORMAT
} status_t;

typedef enum {
	FALSE,
	TRUE
} bool_t;

typedef char * string;

#endif 
