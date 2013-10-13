#include <stdbool.h>
#include "types/HashMap.h"

#define NO_PATH (-1)

typedef uintptr_t location_t;

typedef location_t *path_t;

typedef struct {
	location_t location;
	int cost;
} expansion_t;

typedef int (*expanderDelegate)(location_t, expansion_t **, void *);
typedef int (*heuristicDelegate)(location_t, void *);
typedef bool (*goalDelegate)(location_t, void *);

int aStar_shortestPath(
	location_t start,
	goalDelegate goalCondition,
	path_t *path,

	expanderDelegate expander,
	heuristicDelegate heuristic,
	hashDelegate hashFunction,
	eqDelegate isEqual,

	void *data
);
