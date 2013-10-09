#include "types/PQueue.h"
#include "types/HashMap.h"
#include "types/List.h"
#include "pathfinding/aStar.h"

#define MAP_SIZE 1024
#define VISITED 1

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct {
	location_t location;
	location_t cameFrom;
	int stepsToStart;
	int costToStart;
	int estimatedPathCost;
} pathNode;

static int buildPath(pathNode *goalNode, path_t *path);

int aStar_shortestPath(
	location_t start,
	goalDelegate goalCondition,
	path_t *path,

	expanderDelegate expander,
	heuristicDelegate heuristic,
	hashDelegate hashFunction,
	eqDelegate isEqual,
	
	void *data
) {
	HashMap closedSet;
	PQueue openSet;

	pathNode *currentNode;
	pathNode *neighbourNode;
	expansion_t *neighbours;
	int numNeighbours;
	int i;

	int costToStart;
	int pathEstimate;

	int pathLength = NO_PATH;

	List pathNodes;

	pathNodes = new_List();

	closedSet = new_HashMap(hashFunction, isEqual, MAP_SIZE);
	currentNode = malloc(sizeof(pathNode));
	assert(currentNode != NULL);

	currentNode->location = start;
	currentNode->cameFrom = (location_t)NULL;
	currentNode->costToStart = 0;
	currentNode->stepsToStart = 0;
	currentNode->estimatedPathCost = heuristic(start, data);

	append_List(pathNodes, currentNode);

	openSet = new_PQueue();
	add_PQueue(openSet, currentNode, -currentNode->estimatedPathCost);

	while (size_PQueue(openSet) > 0) {
		currentNode = remove_PQueue(openSet);

		if (goalCondition(currentNode->location, data)) {
			pathLength = buildPath(currentNode, path);
			break;
		}

		set_HashMap(closedSet, (uintptr_t)currentNode->location, (uintptr_t)currentNode);

		numNeighbours = expander(currentNode->location, neighbours, data);

		for (i = 0; i != numNeighbours; ++i) {
			costToStart = currentNode->costToStart + neighbours[i].cost;
			pathEstimate = costToStart + heuristic(neighbours[i].location, data);

			neighbourNode = (pathNode *)get_HashMap(closedSet, neighbours[i].location);
			if (neighbourNode == NULL || pathEstimate < neighbourNode->estimatedPathCost) {
				neighbourNode = malloc(sizeof(pathNode));
				assert(neighbourNode != NULL);

				neighbourNode->location = neighbours[i].location;
				neighbourNode->cameFrom = currentNode->location;
				neighbourNode->stepsToStart = currentNode->stepsToStart + 1;
				neighbourNode->costToStart = costToStart;
				neighbourNode->estimatedPathCost = pathEstimate;

				append_List(pathNodes, neighbourNode);
				add_PQueue(openSet, neighbourNode, -pathEstimate);
			}
		}

		free(neighbours);
	}

	while (size_List(pathNodes) > 0) {
		currentNode = (pathNode *)removeFirst_List(pathNodes);
		free(currentNode);
	}
	destroy_List(pathNodes);
	destroy_HashMap(closedSet);
	destroy_PQueue(openSet);

	return pathLength;
}


static int buildPath(pathNode *goalNode, path_t *path) {
	int pathLength;
	int i;
	pathNode *node = goalNode;


	pathLength = node->stepsToStart + 1;
	*path = malloc(sizeof(location_t) * pathLength);
	i = 0;
	while (node != NULL) {
		*path[pathLength-i-1] = node->location;
		node = node->cameFrom;
		++i;
	}

	return pathLength;
}
