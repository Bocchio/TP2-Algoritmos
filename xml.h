#ifndef XML__H
#define XML__H

#include "types.h"

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"

typedef struct{
	bool_t print_header;
	string previous_chunk;
	string next_chunk;
	string attributes;
	string label;
	uchar indentation;
} xml_ctx_t;

status_t xml_print_header(FILE *output_file);
status_t xml_open_tag(string label, string attributes, uchar tabs, FILE *output_file);
status_t xml_close_tag(string label, uchar tabs, FILE *output_file);

#endif