#include <stdio.h>
#include "errors.h"
#include "types.h"
#include "lang_support.h"

char *errors[] = {
	MSG_OK,
	MSG_ERROR_NULL_POINTER,
	MSG_ERROR_MEMORY,
	ERROR_UNKNOWN_FILE_FORMAT
};

status_t show_error(status_t st)
{
	fprintf(stderr, "%s\n", errors[st]);
	return OK;
}