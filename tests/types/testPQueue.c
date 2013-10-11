#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "types/PQueue.h"

int main(int argc, char *argv[]) {
   PQueue pq;

   printf("Testing PQueue\n");

   pq = new_PQueue();

   char v1 = 'a';
   char v2 = 'b';
   char v3 = 'c';
   char v4 = 'd';

   int p1 = 2;
   int p2 = 4;
   int p3 = 3;
   int p4 = 1;

   add_PQueue(pq, (any_t)&v1, p1);
   add_PQueue(pq, (any_t)&v2, p2);
   add_PQueue(pq, (any_t)&v3, p3);
   add_PQueue(pq, (any_t)&v4, p4);

   assert(*(char *)remove_PQueue(pq) == v2);
   assert(*(char *)remove_PQueue(pq) == v3);
   assert(*(char *)remove_PQueue(pq) == v1);
   assert(*(char *)remove_PQueue(pq) == v4);

   destroy_PQueue(pq);

   printf("All Tests Passed\n");

   return EXIT_SUCCESS;
}
