#include "types.h" 

#define MAX_ARGS    6

#define CMD_ARG_FILE_FORMAT_TOKEN	  	"-fmt"	
#define CMD_ARG_OUTPUT_FILE_TOKEN      	 "-out"

#define OUTPUT_DECIMAL_PRECISION 2

status_t validate_arguments(int argc, char *argv[]);