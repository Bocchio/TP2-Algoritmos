status_t xml_print_header(FILE *output_file)
{
	if(fputs(XML_HEADER, output_file) == EOF)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t xml_open_tag(const string label, const string attributes, size_t tabs, FILE *output_file)
{
	size_t i;

	if(label == NULL || output_file == NULL)
		return ERROR_NULL_POINTER;

	if(attributes == NULL)
		attributes = "";

	for(i = 0; i < tabs; i++){
		if(putc('\t') == EOF)
			return ERROR_WRITING_FILE;
	}

	if(fprintf(output_file, "%s %s %s", "<", label, attributes, ">") < 0)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t xml_close_tag(string label, size_t tabs, FILE *output_file)
{
	size_t i;

	for(i = 0; i < tabs; i++){
		if(putc('\t') == EOF)
			return ERROR_WRITING_FILE;
	}
	
	if(fprintf(output_file, "%s%s %s", "</", label, ">") < 0)
		return ERROR_WRITING_FILE;

	return OK;
}