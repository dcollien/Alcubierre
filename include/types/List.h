#ifndef LIST_H
#define LIST_H
#include <stdint.h>
#include <stdbool.h>

typedef struct _list *List;

List newList(void);

void destroy_List(List list);

void prepend_List(List list, uintptr_t value);
void append_List(List list, uintptr_t value);

uintptr_t removeFirst_List(List list);
uintptr_t removeLast_List(List list);

unsigned int size_List(List list);

#endif