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

    if(src == NULL || target == NULL)
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

/* Checks if the string s1 starts with the string s2 */
bool_t starts_with(const char * s1, const char * s2)
{
    size_t i;

    for(i = 0; s2[i]; i++){
        if(s1[i] != s2[i])
            return FALSE;
    }

    return TRUE;
}


/* Creates a dynamic array of the substrings in "src" using the
   separator string "delim". Appends NULL at the end of that array.
   For example:
       string *dest;
       split("hello worldsepsep!!sep", &dest, "sep");
   would put {"hello world", "", "!!", "", NULL} inside dest
*/
status_t split(const char * src, char ***dest, char *delim, size_t *number_of_substrings)
{
    status_t st;
    size_t i, j;
    size_t delim_len;
    char * copy;
    char * aux;

    if(src == NULL || dest == NULL || delim == NULL || number_of_substrings == NULL)
        return ERROR_NULL_POINTER;

    if((st = strdup(src, &copy)) != OK){
        return st;
    }

    delim_len = strlen(delim);
    /* Count the total number of substrings and NUL terminate them */
    for(i = 0, *number_of_substrings = 1; copy[i]; i++){
        if(starts_with(copy + i, delim) == FALSE)
            continue;
        /* If the delimiter string was found, NUL terminate the substring
         * then jump to the end of the delimiter */
        copy[i] = '\0';
        (*number_of_substrings)++;
        /* The for post-increment will add 1, so we substract 1 to compensate */
        i += delim_len - 1;
    }

    /* Take into account the the NULL pointer at the end */
    if((*dest = (char **) malloc((*number_of_substrings + 1)*sizeof(char *))) == NULL)
        return ERROR_MEMORY;

    /* Copy each substring into the array of strings */
    for(i = 0; i < *number_of_substrings; i++){
        if((st = strdup(copy, &aux)) != OK){
            for(j = 0; j < i; j++)
                free((*dest)[j]);
            free(*dest);
            free(copy);
            return st;
        }
        (*dest)[i] = aux;
        copy += strlen(aux) + delim_len;
    }
    (*dest)[i] = NULL;

    return OK;
}

/* Receives a pointer to a string array and frees it */
status_t free_string_array(string **s,size_t len)
{
    size_t i;

    if(s == NULL)
        return OK;

    for(i = 0; i < len; i++){
        if((*s)[i] == NULL)
            continue;
        free((*s)[i]);
        (*s)[i]=NULL;
    }
    free(*s);
    *s = NULL;

    return OK;
}
