#ifndef UTILS__H
#define UTILS__H

#include <stdio.h>
#include "types.h"

#define INIT_CHOP 	15
#define CHOP_SIZE 	20

status_t readline(FILE *fi, char **s, bool_t *eof);
status_t readfile(FILE *fi, char **s);
status_t strdup(const char * src, char **target);
bool_t starts_with(const char * s1, const char * s2);
status_t split(const char * src, char ***dest, char *delim, size_t * substrings_number);
status_t free_string_array(string **s, size_t len);

#endif
