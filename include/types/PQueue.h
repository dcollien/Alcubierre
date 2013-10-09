typedef struct _pq PQueue;

PQueue new_PQueue(int maxSize);

void delete_PQueue(PQueue pq);

void *remove_PQueue(PQueue pq);

void add_PQueue(PQueue pq, void *toAdd, int priority);

