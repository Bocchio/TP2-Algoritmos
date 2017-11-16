#ifndef CONFIG__H
#define CONFIG__H

#include "types.h"

typedef struct{
	string fi_path;
	string fo_path;
	file_format_t file_format;
} config_t;

#endif