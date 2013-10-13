#ifndef PQUEUE_H
#define PQUEUE_H
#include "types/any.h"

typedef struct _pq *PQueue;

PQueue new_PQueue(void);

void destroy_PQueue(PQueue pq);

any_t remove_PQueue(PQueue pq);

void add_PQueue(PQueue pq, any_t toAdd, int priority);

unsigned int size_PQueue(PQueue pq);

void printPriorities_PQueue(PQueue pq, anyPrintDelegate printer);
#endif
