#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "types.h"

status_t readline(FILE *fi, string *s, bool_t *eof)
{
	size_t alloc_size;
	size_t len;
	string aux;
	int c;
	
	if(fi == NULL || s == NULL || eof == NULL)
		return ERROR_NULL_POINTER;

	if((*s = malloc(INIT_CHOP*sizeof(char))) == NULL)
		return ERROR_MEMORY;

	alloc_size = INIT_CHOP;
	len = 0;

	while((c = fgetc(fi)) != '\n' && c != EOF){
		/* Check if we need more space taking into account the future NUL terminator */
		if(len == alloc_size - 1){
			if((aux = realloc(*s, (alloc_size + CHOP_SIZE)*sizeof(char))) == NULL){
				free(*s);
				*s=NULL;
				return ERROR_MEMORY;
			}
			alloc_size += CHOP_SIZE;
			*s = aux;
		}
		(*s)[len++] = c;
	}
	(*s)[len] = '\0';

	/* Free unused memory */
	if(len + 1 < alloc_size){
		if((aux = realloc(*s, (len + 1)*sizeof(char))) == NULL){
			free(*s);
			*s=NULL;
			return ERROR_MEMORY;
		}
		*s = aux;
	}
	
	*eof = (c == EOF) ? TRUE : FALSE;

	return OK;
}

status_t load_text_file(FILE *fi, string *s)
{
	size_t alloc_size;
	size_t len;
	string aux;
	int c;
	
	if(fi == NULL || s == NULL)
		return ERROR_NULL_POINTER;

	if((*s = malloc(INIT_CHOP*sizeof(char))) == NULL)
		return ERROR_MEMORY;

	alloc_size = INIT_CHOP;
	len = 0;

	while((c = fgetc(fi)) != EOF){
		/* Check if we need more space taking into account the future NUL terminator */
		if(len == alloc_size - 1){
			if((aux = realloc(*s, (alloc_size + CHOP_SIZE)*sizeof(char))) == NULL){
				free(*s);
				*s=NULL;
				return ERROR_MEMORY;
			}
			alloc_size += CHOP_SIZE;
			*s = aux;
		}
		(*s)[len++] = c;
	}
	(*s)[len] = '\0';

	/* Free unused memory */
	if(len + 1 < alloc_size){
		if((aux = realloc(*s, (len + 1)*sizeof(char))) == NULL){
			free(*s);
			*s=NULL;
			return ERROR_MEMORY;
		}
		*s = aux;
	}
	
	return OK;
}

status_t strdup(const char *src, char **target)
{
	size_t i;
	size_t len;
	
	if(src==NULL||target==NULL)
		return ERROR_NULL_POINTER;
	len = strlen(src);
	if((*target = malloc((len + 1)*sizeof(char))) == NULL){
		return ERROR_MEMORY;
	}

	for(i = 0; src[i]; i++)
		(*target)[i] = src[i];
	(*target)[i] = '\0';

	return OK;
}


/* Creates a dynamic array of the substrings in "src" using the
   separator string "delim". Appends NULL at the end of that array.
   For example:
   	   string *dest; 
   	   split("hello worldsepsep!!sep", &dest, "sep");
   would put {"hello world", "", "!!", "", NULL} inside dest
*/
status_t split(const char * src, char ***dest, char *delim,size_t * substrings_number)
{
	status_t st;
	size_t i, j;
	size_t delim_len;
	bool_t found_delim;
	char * copy;
	char * aux;

	if(src==NULL || dest==NULL || *dest==NULL || delim==NULL)
		return ERROR_NULL_POINTER;
	if((st = strdup(src, &copy)) != OK){
		return st;
	}

	delim_len = strlen(delim);
	/* Count the total number of substrings and NUL terminate them */
	for(i = 0, *substrings_number = 1, j = 0; copy[i]; i++){
		/* If we found the delimiter, increment the number of substrings */
		for(; copy[j+i]; j++){
			if(copy[j+i]==delim[j]){
				(*substrings_number)++;
				found_delim=TRUE;
				break;
			}
		}
		/* If the delimiter string was found, NUL terminate the substring
		 * then jump to the end of the delimiter */
		if(found_delim == TRUE){
			copy[i] = '\0';
			i += delim_len - 1;
		}
	}

	/* Take into account the the NULL pointer at the end */
	if((*dest = (char **) malloc((substrings + 1)*sizeof(char *))) == NULL)
		return ERROR_MEMORY;

	/* Copy each substring into the array of strings */
	for(i = 0, j = 0; j < substrings; i++, j++){
		if((st = strdup(copy + i, &aux)) != OK){
			for(i = 0; i < j; i++)
				free((*dest)[i]);
			free(*dest);
			return st;
		}
		(*dest)[j] = aux;
		i += strlen(aux) + delim_len - 1;
	}
	(*dest)[j] = NULL;

	return OK;
}

/* Receives a pointer to a string array and frees it */
status_t free_string_array(string **s,size_t len)
{
	size_t i;

	for(i = 0; i < len; i++){
		free((*s)[i]);
		(*s)[i]=NULL;
	}
	free(*s);
	*s = NULL;

	return OK;
}
