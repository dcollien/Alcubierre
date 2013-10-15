#ifndef ANY_H
#define ANY_H
#include <stdint.h>
#include <stdbool.h>

typedef uintptr_t any_t;
typedef void (*anyPrintDelegate)(any_t);
typedef bool (*anyEqDelegate)(any_t, any_t);

#endif
