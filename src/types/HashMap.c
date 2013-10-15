#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "types/HashMap.h"

typedef struct collisionNode collisionNode_t;

struct collisionNode {
   any_t key;
   any_t value;
   collisionNode_t *next;
};

struct _hashmap {
   collisionNode_t **array;
   int size;
   hashDelegate hashFunction;
   anyEqDelegate isEqual;
};

static bool isEqual(HashMap map, any_t valueA, any_t valueB);

static void clearNodes(HashMap map, unsigned int index);

HashMap new_HashMap(hashDelegate hashFunction, anyEqDelegate isEqual, size_t size) {
   HashMap map = malloc(sizeof(struct _hashmap));
   assert(map != NULL);

   map->size = size;
   map->array = malloc(sizeof(collisionNode_t *) * size);
   assert(map->array != NULL);

   memset(map->array, (uintptr_t)NULL, sizeof(collisionNode_t *) * size);

   map->hashFunction = hashFunction;
   map->isEqual = isEqual;

   return map;
}

void destroy_HashMap(HashMap map) {
   empty_HashMap(map);
   
   free(map->array);
   free(map);
}

void remove_HashMap(HashMap map, any_t key) {
   unsigned int hash = map->hashFunction(key) % map->size;
   collisionNode_t *node = map->array[hash];
   collisionNode_t *prev = NULL;

   while (node != NULL && !isEqual(map, node->key, key)) {
      node = node->next;
   }

   if (node != NULL && isEqual(map, node->key, key)) {
      if (prev == NULL) {
         map->array[hash] = node->next;
      } else {
         prev->next = node->next;
      }
      free(node);
   }
}

void set_HashMap(HashMap map, any_t key, any_t value) {
   unsigned int hash = map->hashFunction(key) % map->size;
   collisionNode_t *prevNode;
   collisionNode_t *node = malloc(sizeof(collisionNode_t));
   assert(node != NULL);

   node->key = key;
   node->value = value;
   node->next = NULL;

   prevNode = map->array[hash];
   if (prevNode == NULL) {
      map->array[hash] = node;
   } else {
      while (prevNode->next != NULL) {
         if (isEqual(map, prevNode->key, key)) {
            break;
         }
         prevNode = prevNode->next;
      }

      if (isEqual(map, prevNode->key, key)) {
         prevNode->value = node->value;
         free(node);
      } else {
         prevNode->next = node;
      }
   }
}

any_t get_HashMap(HashMap map, any_t key) {
   unsigned int hash = map->hashFunction(key) % map->size;
   collisionNode_t *node = map->array[hash];
   any_t value;

   while (node != NULL && !isEqual(map, node->key, key)) {
      node = node->next;
   }

   if (node == NULL || !isEqual(map, node->key, key)) {
      value = (any_t)NULL;
   } else {
      value = node->value;
   }

   return value;
}


bool in_HashMap(HashMap map, any_t key) {
   unsigned int hash = map->hashFunction(key) % map->size;
   collisionNode_t *node = map->array[hash];

   while (node != NULL) {
      if (isEqual(map, node->key, key)) {
         return true;
      }
      node = node->next;
   }

   return false;
}

void empty_HashMap(HashMap map) {
   int i;

   for (i = 0; i != map->size; ++i) {
      clearNodes(map, i);
   }
}

static bool isEqual(HashMap map, any_t valueA, any_t valueB) {
   anyEqDelegate eqFunc = map->isEqual;

   if (eqFunc == NULL) {
      return (valueA == valueB);
   } else {
      return eqFunc(valueA, valueB);
   }
}

static void clearNodes(HashMap map, unsigned int index) {
   collisionNode_t *prev;
   collisionNode_t *node = map->array[index];

   while (node != NULL) {
      prev = node;
      node = node->next;
      free(prev);
   }

   map->array[index] = NULL;
}

