#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdint.h>
#include <stdbool.h>

typedef struct _hashmap *HashMap;

typedef unsigned int (*hashDelegate)(uintptr_t);
typedef bool (*eqDelegate)(uintptr_t, uintptr_t);

HashMap new_HashMap(hashDelegate hashFunction, eqDelegate isEqual, size_t size);

void destroy_HashMap(HashMap map);

void set_HashMap(HashMap map, uintptr_t key, uintptr_t value);

uintptr_t get_HashMap(HashMap map, uintptr_t key);

bool in_HashMap(HashMap map, uintptr_t key);

#endif