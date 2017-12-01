#ifndef CONFIG__H
#define CONFIG__H

#include <stdio.h>
#include "types.h"

typedef struct{
	string fi_path;
	string fo_path;
	doc_type_t file_format;
} config_t;

#endif
