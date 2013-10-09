#ifndef PQUEUE_H
#define PQUEUE_H

typedef struct _pq *PQueue;
typedef void *pq_value_t;

PQueue new_PQueue(void);

void destroy_PQueue(PQueue pq);

pq_value_t remove_PQueue(PQueue pq);

void add_PQueue(PQueue pq, pq_value_t toAdd, int priority);

unsigned int size_PQueue(PQueue pq);
#endif
