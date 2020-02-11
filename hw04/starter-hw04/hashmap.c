
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// TODO: sudo apt install l	ibbsd-dev
// This provides strlcpy
// See "man strlcpy"
#include <bsd/string.h>
#include <string.h>

#include "hashmap.h"


int
hash(char* key)
{
    // TODO: Produce an appropriate hash value.
    int h = 0;
    for(int i = 0; key[i]; ++i) {
        h = h * 67 + key[i];
    }
    return h;
}	

hashmap*
make_hashmap_presize(int nn)
{
    hashmap* hh = calloc(1, sizeof(hashmap));
    // TODO: Allocate and initialize a hashmap with capacity 'nn'.
    // Double check "man calloc" to see what that function does.
    hh->size = 0;
    hh->capacity = nn;
    hh->data = calloc(nn, sizeof(hashmap_pair));
    for(int i = 0; i < nn; ++i) {
	hashmap_pair* pair= &hh->data[i];
        pair->used = false;
        pair->tomb = false;
    }
    
    return hh;
}

hashmap*
make_hashmap()
{
    return make_hashmap_presize(4);
}

void
free_hashmap(hashmap* hh)
{
    // TODO: Free all allocated data.
    free(hh->data);
    free(hh);
}

int
hashmap_has(hashmap* hh, char* kk)
{
    return hashmap_get(hh, kk) != -1;
}

int
hashmap_get(hashmap* hh, char* kk)
{
    // TODO: Return the value associated with the
    // key kk.
    // Note: return -1 for key not found.
    int h = hash(kk);
    int hash = h & (hh->capacity - 1);

    hashmap_pair* pair= &hh->data[hash];
    while(pair->used && !pair->tomb) {
        pair= &hh->data[hash];
        if(strcmp(pair->key, kk) == 0) {
            return pair->val;
        }
        hash = (hash + 1) % hh->capacity;
    }
    return -1;
}

void
hashmap_put(hashmap* hh, char* kk, int vv)
{

    if(hh->size > hh->capacity * 0.5) {
        //grow
        hh->capacity *= 2;
        hh->size = 0;
        hashmap_pair* data = hh->data;
        hashmap_pair* newData = calloc(hh->capacity, sizeof(hashmap_pair));
        
        hh->data = newData;
        for(int i = 0; i < hh->capacity / 2; ++i) {
            if(!data[i].used || data[i].tomb) {
                continue;
            }
            hashmap_put(hh, data[i].key, data[i].val);
        }

        free(data);
    }


    // TODO: Insert the value 'vv' into the hashmap
    // for the key 'kk', replacing any existing value
    // for that key.

    hh->size++;

    int h = hash(kk);
    int hash = h & (hh->capacity - 1);

//go back to top of list
    while(true) {
        hashmap_pair* pair= &hh->data[hash];

        if(!pair->used || pair->tomb || strcmp(pair->key, kk) == 0) {
            strlcpy(pair->key, kk, 4);
            pair->val = vv;
	    pair->used = true;
            pair->tomb = false;
            return;
        }
        hash = (hash + 1) % hh->capacity;
    }

}

void
hashmap_del(hashmap* hh, char* kk)
{
    // TODO: Remove any value associated with
    // this key in the map.
 
    hh->size--;

    int h = hash(kk);
    int hash = h & (hh->capacity - 1);

    hashmap_pair* pair= &hh->data[hash];
    while(pair->used && !pair->tomb) {
        pair= &hh->data[hash];

        if(strcmp(pair->key, kk) == 0) {
            pair->tomb = true;
            return;
        }
        hash = (hash + 1) % hh->capacity;
    }
    //key not exist
    return;
    
}

hashmap_pair
hashmap_get_pair(hashmap* hh, int ii)
{
    // TODO: Get the {k,v} pair stored in index 'ii'.
    return hh->data[ii];
}

void
hashmap_dump(hashmap* hh)
{
    printf("== hashmap dump ==\n");
    // TODO: Print out all the keys and values currently
    // in the map, in storage order. Useful for debugging.
    for(int i = 0; i < hh->capacity; ++i) {
        hashmap_pair* pair= &hh->data[i];
        printf("key: %s, val: %d\n", pair->key, pair->val);
    }
    puts("\n");
}
