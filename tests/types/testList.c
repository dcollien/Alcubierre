#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "types/List.h"


int main(int argc, char *argv[]) {
   List list;

   printf("Testing List\n");

   list = new_List();

   append_List(list, 42);
   prepend_List(list, 12);
   prepend_List(list, 81);
   append_List(list, 69);
   assert(size_List(list) == 4);
   
   assert(removeFirst_List(list) == 81);
   assert(removeFirst_List(list) == 12);
   assert(removeLast_List(list) == 69);
   assert(removeFirst_List(list) == 42);
   assert(size_List(list) == 0);
   
   destroy_List(list);

   printf("All Tests Passed\n");

   return EXIT_SUCCESS;
}
