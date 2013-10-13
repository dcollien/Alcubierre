#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "types/PQueue.h"

#define START_SIZE 128

typedef struct {
   any_t value;
   int priority;
} pq_node;

typedef pq_node *pq_node_ptr;

struct _pq {
   pq_node_ptr *elements;
   int size;
   int length; 
};

static void heapify(PQueue pq, int i);

PQueue new_PQueue(void) {
   PQueue pq = malloc(sizeof(struct _pq));
   assert(pq != NULL);

   pq->elements = malloc(sizeof(pq_node_ptr) * START_SIZE);
   assert(pq->elements != NULL);

   pq->size = START_SIZE;
   pq->length = 0;

   return pq;
}

void destroy_PQueue(PQueue pq) {
   while (pq->length > 0) {
      remove_PQueue(pq);
   }
   free(pq->elements);
   free(pq);
}

any_t remove_PQueue(PQueue pq) {
   assert(pq->length > 0);

   // store the top value in the heap
   pq_node_ptr removed = pq->elements[0];
   any_t removedValue = removed->value;

   free(removed);

   // move the last to the front
   pq->elements[0] = pq->elements[pq->length-1];
   --(pq->length);

   // heapify the front node
   heapify(pq, 0);

   return removedValue;
}

void add_PQueue(PQueue pq, any_t toAdd, int priority) {
   int i;
   int parent;

   pq_node_ptr *heap;

   if (pq->length >= pq->size) {
      pq->size *= 2;
      pq->elements = realloc(pq->elements, sizeof(pq_node_ptr) * pq->size);
   }

   heap = pq->elements;

   // create node and store properties
   pq_node_ptr newNode = malloc(sizeof(pq_node));
   assert(newNode != NULL);

   newNode->priority = priority;
   newNode->value = toAdd;

   ++(pq->length);
   i = pq->length - 1;

   parent = i/2;
   while (i > 0 && heap[parent]->priority < priority) {
      // move the parent node down to make space
      heap[i] = heap[parent];

      i = parent;
      parent = i/2;
   }

   // insert the new node here
   heap[i] = newNode;
}

unsigned int size_PQueue(PQueue pq) {
   return pq->length;
}

void printPriorities_PQueue(PQueue pq, anyPrintDelegate printer) {
   int i;
   int space;
   int spacing;
   int col;
   int rowLength;

   spacing = pq->length/2;
   rowLength = 1;
   i = 0;
   while (i < pq->length) {

      for (col = 0; col < rowLength && i < pq->length; ++col) {
         for (space = 0; space != spacing; ++space) {
            printf("   ");
            if (printer != NULL) {
               printer((any_t)NULL);
            }
         }
         
         printf("%3d", pq->elements[i]->priority);

         if (printer != NULL) {
            printer(pq->elements[i]->value);
         }

         i++;
      }

      spacing /= 2;
      rowLength *= 2;
      printf("\n");
   }
}

static void heapify(PQueue pq, int i) {
   // indexes of the heap
   int greatest = i;
   int left = 2 * i;
   int right = 2 * i + 1;

   int length = pq->length;
   pq_node_ptr *heap = pq->elements;
   pq_node_ptr swappedNode;

   if (left < length && heap[left]->priority > heap[i]->priority) {
      greatest = left;
   }

   if (right < length && heap[right]->priority > heap[greatest]->priority) {
      greatest = right;
   }

   if (greatest != i) {
      swappedNode = heap[i];
      heap[i] = heap[greatest];
      heap[greatest] = swappedNode;

      heapify(pq, greatest);
   }
}

