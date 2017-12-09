#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "utils.h"
#include "vector.h"

status_t ADT_Vector_new(ADT_Vector_t **vector)
{
    if(vector == NULL)
        return ERROR_NULL_POINTER;

    if((*vector = (ADT_Vector_t *) malloc(sizeof(ADT_Vector_t))) == NULL)
        return ERROR_MEMORY;

    if(((*vector)->elements=(void **)malloc(sizeof(void *)*ADT_Vector_INIT_CHOP))==NULL){
        free(*vector);
        *vector=NULL;
        return ERROR_MEMORY;
    }
    (*vector)->alloc_size = ADT_Vector_INIT_CHOP;
    (*vector)->len = 0;
    (*vector)->tag_name = "";
    (*vector)->delete_element = NULL;
    (*vector)->clone_element = NULL;
    (*vector)->export_element_as_csv = NULL;
    (*vector)->export_element_as_kml = NULL;

    return OK;
}

status_t ADT_Vector_destroy(ADT_Vector_t **vector)
{
    size_t i;
    status_t st;

    if(vector == NULL)
        return ERROR_NULL_POINTER;

    /* If there's a function to destroy each element */
    if((*vector)->destroy_element != NULL){
        for(i = 0; i < (*vector)->len; i++){
            if((st = (*vector)->destroy_element((*vector)->elements + i)) != OK)
                return st;
        }
    }

    free((*vector)->elements);
    (*vector)->elements = NULL;
    free(*vector);
    *vector = NULL;

    return OK;
}

status_t ADT_Vector_append(ADT_Vector_t *vector, void *new_item)
{
    size_t i;
    void **aux;
    size_t new_alloc_size;

    if(vector == NULL || new_item == NULL)
        return ERROR_NULL_POINTER;

    /* Allocate more space if we need to */
    if(vector->len == vector->alloc_size){
        new_alloc_size = vector->alloc_size + ADT_Vector_CHOP_SIZE;
        if((aux = (void **) realloc(vector->elements, new_alloc_size*sizeof(void *))) == NULL)
            return ERROR_MEMORY;
        vector->elements = aux;
        vector->alloc_size = new_alloc_size;

        /* Set every non-used position to NULL */
        for(i = vector->len; i < vector->alloc_size; i++){
            vector->elements[i] = NULL;
        }
    }

    vector->elements[vector->len] = new_item;
    vector->len++;

    return OK;
}

void * ADT_Vector_get_element(const ADT_Vector_t *vector, size_t pos)
{
    if(vector == NULL)
        return NULL;

    if(pos >= vector->len)
        return NULL;

    return vector->elements[pos];
}

status_t ADT_Vector_export_as_csv(const ADT_Vector_t *vector, void *ctx, FILE *fo)
{
    size_t i;
    status_t st;

    for(i=0; i < vector->len; i++){
        if((st = vector->export_element_as_csv(vector->elements[i], ctx, fo)) != OK)
            return st;
    }

    return OK;
}

status_t ADT_Vector_export_as_kml(const ADT_Vector_t *vector, kml_ctx_t *context, FILE *fo)
{
    size_t i;
    status_t st;
    string header, footer;

    if(vector == NULL || context == NULL || fo == NULL)
        return ERROR_NULL_POINTER;

    if((st = load_text_file(context->header, &header)) != OK){
        return st;
    }

    if((st = load_text_file(context->footer, &footer)) != OK){
        return st;
    }

    /* Print the header */
    if(fputs(context->header, fo) == EOF)
        return ERROR_WRITING_FILE;

    /* Open the vectors tag */
    if(fprintf(fo, "%c%s%c\n", '<', vector->tag_name, '>') < 0)
        return ERROR_WRITING_FILE;

    /* Export each element */
    for(i = 0; i < vector->len; i++){
        if((st = vector->export_element_as_kml(vector->elements[i], NULL, fo)) != OK){
            free(header);
            free(footer);
            return st;
        }
    }

    /* closes the vectors tag */
    if(fprintf(fo, "%s%s%c\n", "</", vector->tag_name, '>') < 0)
        return ERROR_WRITING_FILE;

    /* Print the footer */
    if(fputs(context->footer, fo) == EOF)
        return ERROR_WRITING_FILE;

    return OK;
}

status_t ADT_Vector_set_tag_name(ADT_Vector_t *vector, string label)
{
    if(vector == NULL || label == NULL)
        return ERROR_NULL_POINTER;

    vector->label = label;

    return OK;
}

status_t ADT_Vector_set_destructor(ADT_Vector_t *vector, destructor_t destructor)
{
    if(vector == NULL)
        return ERROR_NULL_POINTER;

    vector->delete_element = destructor;

    return OK;
}

status_t ADT_Vector_set_clonator(ADT_Vector_t *vector, clonator_t clonator)
{
    if(vector == NULL)
        return ERROR_NULL_POINTER;

    vector->clone_element = clonator;

    return OK;
}

status_t ADT_Vector_set_csv_exporter(ADT_Vector_t *vector, printer_t csv_exporter)
{
    if(vector == NULL)
        return ERROR_NULL_POINTER;

    vector->export_element_as_csv = csv_exporter;

    return OK;
}

status_t ADT_Vector_set_kml_exporter(ADT_Vector_t *vector, printer_t kml_exporter)
{
    if(vector == NULL)
        return ERROR_NULL_POINTER;

    vector->export_element_as_kml = kml_exporter;

    return OK;
}
