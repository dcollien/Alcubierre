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

   // TODO

   destroy_List(list);

   printf("All Tests Passed\n");

   return EXIT_SUCCESS;
}
