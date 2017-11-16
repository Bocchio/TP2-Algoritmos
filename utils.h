#ifndef UTILS__H
#define UTILS__H

#include <stdio.h>
#include "types.h"

#define INIT_CHOP	20
#define CHOP_SIZE	25

status_t readline(FILE *fi, char **s, bool_t *eof);
status_t strdup(const_string src, string *dest);
status_t split(const_string src, string **dest, string delim);
status_t free_string_array(string **s);

#endif