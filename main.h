#include <stdio.h>
#include "types.h" 
#include "config.h"

#define MAX_ARGS    6

#define CMD_ARG_INPUT_FILE_POS		5	
#define CMD_ARG_FILE_FORMAT_TOKEN	"-fmt"	
#define CMD_ARG_OUTPUT_FILE_TOKEN   "-out"

#define FILE_FORMAT_CSV_FLAG	"csv"
#define FILE_FORMAT_KML_FLAG	"kml"

status_t validate_arguments(int argc, char *argv[], config_t *config);
