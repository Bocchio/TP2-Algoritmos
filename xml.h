#ifndef XML__H
#define XML__H

#include "types.h"

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"

status_t xml_print_header(FILE *output_file);
status_t xml_open_tag(string label, string attributes, size_t tabs, FILE *output_file);
status_t xml_close_tag(string label, size_t tabs, FILE *output_file);

#endif