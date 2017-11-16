#include <stdio.h>
#include "types.h"
#include "xml.h"

status_t xml_print_header(FILE *output_file)
{
	if(fputs(XML_HEADER, output_file) == EOF)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t xml_open_tag(const_string label, const_string attrib, uchar tabs, FILE *fo)
{
	uchar i;

	if(label == NULL || fo == NULL)
		return ERROR_NULL_POINTER;

	if(!*label)
		return ERROR_INVALID_FUNCTION_ARGS;

	if(attrib == NULL)
		attrib = "";

	for(i = 0; i < tabs; i++){
		if(putc('\t', fo) == EOF)
			return ERROR_WRITING_FILE;
	}

	if(fprintf(fo, "%s%s%s%s\n", "<", label, attrib, ">") < 0)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t xml_close_tag(const_string label, uchar tabs, FILE *fo)
{
	uchar i;

	for(i = 0; i < tabs; i++){
		if(putc('\t', fo) == EOF)
			return ERROR_WRITING_FILE;
	}
	
	if(fprintf(fo, "%s%s%s\n", "</", label, ">") < 0)
		return ERROR_WRITING_FILE;

	return OK;
}