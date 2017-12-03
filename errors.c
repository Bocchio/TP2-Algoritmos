#include <stdio.h>
#include "errors.h"
#include "types.h"
#include "lang_support.h"

char *errors[MAX_ERRORS] = {
	MSG_OK,
	MSG_ERROR_NULL_POINTER,
	MSG_ERROR_MEMORY,
	MSG_ERROR_OPENING_FILE,
	MSG_ERROR_READING_FILE,
	MSG_ERROR_WRITING_FILE,
	MSG_ERROR_PROGRAM_INVOCATION,
	MSG_ERROR_UNKNOWN_FILE_FORMAT
};

status_t show_error(status_t st)
{
	fprintf(stderr, "%s\n", errors[st]);
	return OK;
}
