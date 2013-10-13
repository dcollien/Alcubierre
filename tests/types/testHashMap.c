#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "types/HashMap.h"

unsigned int stringHash(any_t value) {
   char *string = (char *)value;
   unsigned int hash = 5381;
   int c;

   while ((c = *string++)) {
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
   }

   return hash;
}

bool stringEq(any_t a, any_t b) {
   return strcmp((char *)a, (char *)b) == 0;
}


int main(int argc, char *argv[]) {
   HashMap map;

   printf("Testing HashMap\n");

   map = new_HashMap(&stringHash, &stringEq, 10);

   char v1 = 'a';
   char v2 = 'b';
   char v3 = 'c';
   char v4 = 'd';

   char k1[] = "aardvark";
   char k2[] = "badger";
   char k3[] = "cat";
   char k4[] = "cat";

   set_HashMap(map, (any_t)k1, (any_t)v1);
   set_HashMap(map, (any_t)k2, (any_t)v2);
   set_HashMap(map, (any_t)k3, (any_t)v3);
   set_HashMap(map, (any_t)k4, (any_t)v4);

   assert((char)get_HashMap(map, (any_t)"aardvark") == v1);
   assert((char)get_HashMap(map, (any_t)"badger") == v2);
   assert((char)get_HashMap(map, (any_t)"cat") == v4);
   assert((void *)get_HashMap(map, (any_t)"zebra") == NULL);

   remove_HashMap(map, (any_t)k3);
   
   assert((void *)get_HashMap(map, (any_t)"cat") == NULL);
   
   destroy_HashMap(map);

   printf("All Tests Passed\n");

   return EXIT_SUCCESS;
}
