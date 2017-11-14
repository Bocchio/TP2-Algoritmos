#ifndef UTILS__H
#define UTILS__H

#include <stdio.h>
#include "types.h"

#define INIT_CHOP	20
#define CHOP_SIZE	25

status_t readline(FILE *fi, char **s, bool_t *eof);
status_t strdup(const string src, string *dest);
status_t split(const string src, string **dest, string delim);

#endif