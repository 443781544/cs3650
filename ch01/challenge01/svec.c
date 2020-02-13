/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "svec.h"

svec*
make_svec()
{
    svec* sv = malloc(sizeof(svec));
    sv->data = malloc(2 * sizeof(char*));
    sv->size = 0;
    sv->capacity = 2;
    // TODO: correctly allocate and initialize data structure
    return sv;
}

void
free_svec(svec* sv)
{
    // TODO: free all allocated data
    for(int i = 0; i < sv->size; ++i) {
        free(sv->data[i]);
    }
    free(sv->data);
    free(sv);
}

char*
svec_get(svec* sv, int ii)
{
    assert(ii >= 0 && ii < sv->size);
    return sv->data[ii];
}

void
svec_put(svec* sv, int ii, char* item)
{
    assert(ii >= 0 && ii < sv->size);
    sv->data[ii] = strdup(item);
    // TODO: insert item into slot ii
    // Consider ownership of string in collection.
}

void
svec_push_back(svec* sv, char* item)
{
    int ii = sv->size;

    // TODO: expand vector if backing erray
    // is not big enough

    sv->size = ii + 1;
    
    if(sv->size > sv->capacity) {
	//double the capacity of array
	sv->capacity *= 2;
	
	//allocate a new array and copy over ii items.
	char** newData = malloc(sv->capacity * sizeof(char*));
	for(int i = 0; i < ii; ++i) {
	    newData[i] = sv->data[i];
	}
	free(sv->data);
	sv->data = newData;
    }
    svec_put(sv, ii, item);
}

void
svec_swap(svec* sv, int ii, int jj)
{
    // TODO: Swap the items in slots ii and jj
    char* temp = sv->data[ii];
    sv->data[ii] = sv->data[jj];
    sv->data[jj] = temp;
    

}

void 
print_svec(svec* xs)
{

/*
    for (int i = 0; i < xs->size; ++i) {
        printf("%s\n", xs->data[i]);
    }
*/

printf("---------------------The List---------------------\n");
    for (int i = 0; i < xs->size; ++i) {
        printf("%s, ", xs->data[i]);
    }
printf("\n");	


}

svec* 
reverse_svec(svec* xs)
{
    svec* rev = make_svec();

    for (int i = (xs->size) - 1; i >= 0; --i) {
        svec_push_back(rev, xs->data[i]);
    }
    return rev;
}
