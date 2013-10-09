#include <assert.h>

#include "PQueue.h"

struct _pq {
	void *elements;
	int size;
	int length;	
};

PQueue new_PQueue(int maxSize) {
	PQueue pq = malloc(sizeof(struct _pq));
	assert(pq != NULL);

	pq->elements = malloc(sizeof(void *) * maxSize);
	assert(pq->elements != NULL);

	pq->size = maxSize;
	pq->length = 0;

	return pq;
}

void delete_PQueue(PQueue pq) {
	free(pq->elements);
	free(pq);
}

void *remove_PQueue(PQueue pq) {
	
}

void add_PQueue(PQueue pq, void *toAdd, int priority);

