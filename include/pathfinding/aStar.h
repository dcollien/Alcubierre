#ifndef ASTAR_H
#define ASTAR_H

#include <stdbool.h>
#include "types/HashMap.h"

#define NO_PATH (-1)

typedef uintptr_t location_t;

typedef location_t *path_t;

typedef struct {
	location_t location;
	int cost;
} expansion_t;

// allocates an array (expansion_t *), populates it with expansion_t
// for a given location_t. returns length of array.
// It's given data, as passed to aStar_shortestPath
typedef int (*expanderDelegate)(location_t, expansion_t **, void *);

// returns an (under) estimate of the distance from the given location_t
// to the nearest goal. It's given data, as passed to aStar_shortestPath
typedef int (*heuristicDelegate)(location_t, void *);

// returns true if the given location_t is a goal. false otherwise.
// It's given data, as passed to aStar_shortestPath
typedef bool (*goalDelegate)(location_t, void *);

/**
 * @brief Finds the shortest path from a starting location to a location
 *        that satisfies a given goal condition
 * @param start the starting location
 * @param goalCondition the condition a location must satisfy to be a goal
 * @param path a pointer to the path array that will be allocated and populated
 *             by this function. This array must be freed externally.
 * @param expander a function which expands a location, specifying its neighbours.
                   This shortestPath function frees the allocated (expansion_t *) array.
 * @param heuristic a function that estimates the distance of a given location to the
 *                  nearest goal location
 * @param hashFunction a hashing function for a location
 * @param isEqual a function that determines if two locations are equal
 * @return the length of the path found, or NO_PATH if none found (path array
 *         is not allocated in this instance and need not be freed)
 */
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

#endif
