#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdlib.h>
#include <stdbool.h>
#include "types/any.h"

typedef struct _hashmap *HashMap;

typedef unsigned int (*hashDelegate)(any_t);
typedef bool (*eqDelegate)(any_t, any_t);


// Constructor and Deconstructor
HashMap new_HashMap(hashDelegate hashFunction, eqDelegate isEqual, size_t size);

void destroy_HashMap(HashMap map);


// Methods
void remove_HashMap(HashMap map, any_t key);

void set_HashMap(HashMap map, any_t key, any_t value);

any_t get_HashMap(HashMap map, any_t key);

bool in_HashMap(HashMap map, any_t key);

#endif
