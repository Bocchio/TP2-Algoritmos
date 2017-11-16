#ifndef TYPES__H
#define TYPES__H

typedef enum {
	OK,
	ERROR_NULL_POINTER,
	ERROR_MEMORY,
	ERROR_OPENING_FILE,
	ERROR_READING_FILE,
	ERROR_WRITING_FILE,
	ERROR_PROGRAM_INVOCATION,
	ERROR_INVALID_FUNCTION_ARGS,
	ERROR_UNKNOWN_FILE_FORMAT
} status_t;

typedef enum {
	FALSE,
	TRUE
} bool_t;

typedef unsigned char uchar;
typedef unsigned int uint;
typedef char * string;
typedef const char * const_string;

typedef enum {
	FORMAT_CSV,
	FORMAT_KML
} file_format_t;

#endif 
