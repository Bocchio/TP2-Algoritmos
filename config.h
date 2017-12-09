#ifndef CONFIG__H
#define CONFIG__H

#include <stdio.h>
#include "types.h"

typedef struct{
	string input_file;
	string output_file;
	doc_type_t output_doc_type;
} config_t;

#endif
