status_t xml_print_header(FILE *output_file)
{
	if(fputs(XML_HEADER, output_file) == EOF)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t xml_open_tag(const string label, const string attributes, FILE *output_file)
{
	if(label == NULL || output_file == NULL)
		return ERROR_NULL_POINTER;

	if(attributes == NULL)
		attributes = "";
	
	if(fprintf(output_file, "%s %s %s", "<", label, attributes, ">") < 0)
		return ERROR_WRITING_FILE;

	return OK;
}

status_t xml_close_tag(string label, FILE *output_file)
{
	if(fprintf(output_file, "%s%s %s", "</", label, ">") < 0)
		return ERROR_WRITING_FILE;

	return OK;
}