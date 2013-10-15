#ifndef LIST_H
#define LIST_H
#include <stdint.h>
#include <stdbool.h>
#include "types/any.h"

typedef struct _list *List;


// Constructor and Deconstructor
List new_List(void);

void destroy_List(List list);

// Methods
void prepend_List(List list, any_t value);
void append_List(List list, any_t value);

any_t removeFirst_List(List list);
any_t removeLast_List(List list);

bool in_List(List list, any_t value, anyEqDelegate isEqualFunc);

unsigned int size_List(List list);

#endif
