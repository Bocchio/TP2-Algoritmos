#include <stdio.h>
#include "types.h"
#include "config.h"

#define MAX_ARGS    6

#define CMD_ARG_INPUT_FILE_POS              5
#define CMD_ARG_OUTPUT_FILE_FORMAT_TOKEN    "-fmt"
#define CMD_ARG_OUTPUT_FILE_TOKEN           "-out"

#define OUTPUT_DOC_TYPE_CSV_FLAG    "csv"
#define OUTPUT_DOC_TYPE_KML_FLAG    "kml"

#define PROGRAM_USAGE_MSG "-fmt <output_format> -out <output_file> <input_file>"

status_t validate_arguments(int argc, char *argv[], config_t *config);
status_t show_usage(string program_name);
