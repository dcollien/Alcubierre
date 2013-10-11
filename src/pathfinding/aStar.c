#include "types/any.h"
#include "types/PQueue.h"
#include "types/HashMap.h"
#include "types/List.h"
#include "pathfinding/aStar.h"

#define MAP_SIZE 1024

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct pathNode pathNode_t;

struct pathNode {
   location_t location;
   pathNode_t *cameFrom;
   int stepsToStart;
   int costToStart;
   int estimatedPathCost;
};

static int buildPath(pathNode_t *goalNode, path_t *path);

/**
 * @brief Finds the shortest path from a starting location to a location
 *        that satisfies a given goal condition
 * @param start the starting location
 * @param goalCondition the condition a location must satisfy to be a goal
 * @param path a pointer to the path array that will be allocated and populated
 *             by this function. This array must be freed externally.
 * @param expander a function which expands a location, specifying its neighbours
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
) {
   HashMap closedSet;
   PQueue openSet;

   pathNode_t *currentNode;
   pathNode_t *neighbourNode;
   expansion_t *neighbours;
   int numNeighbours;
   int i;

   int costToStart;
   int pathEstimate;

   int pathLength = NO_PATH;

   List pathNodes;

   // a list keeping track of all allocated nodes
   pathNodes = new_List();

   // a hashmap of visited nodes
   closedSet = new_HashMap(hashFunction, isEqual, MAP_SIZE);

   // create a node for the starting location
   currentNode = malloc(sizeof(pathNode_t));
   assert(currentNode != NULL);

   currentNode->location = start;
   currentNode->cameFrom = (pathNode_t *)NULL;
   currentNode->costToStart = 0;
   currentNode->stepsToStart = 0;
   currentNode->estimatedPathCost = heuristic(start, data);

   append_List(pathNodes, (uintptr_t)currentNode);

   // a priority queue of unexplored nodes, ranked by cost
   openSet = new_PQueue();
   add_PQueue(openSet, (pq_value_t)currentNode, -currentNode->estimatedPathCost);

   while (size_PQueue(openSet) > 0) {
      // remove the node on the best path so far, thought to be closest to the goal
      currentNode = (pathNode_t *)remove_PQueue(openSet);

      // check if it's a goal, if so build the path
      if (goalCondition(currentNode->location, data)) {
         pathLength = buildPath(currentNode, path);
         break;
      }

      // mark this node as visited, saving it to the closed set
      set_HashMap(closedSet, (uintptr_t)currentNode->location, (uintptr_t)currentNode);

      // expand this node and find its neighbours
      numNeighbours = expander(currentNode->location, neighbours, data);

      for (i = 0; i != numNeighbours; ++i) {
         // for each neighbour determine the cost so far and estimated total
         // path cost
         costToStart = currentNode->costToStart + neighbours[i].cost;
         pathEstimate = costToStart + heuristic(neighbours[i].location, data);

         // see if we've been to this locaton before
         neighbourNode = (pathNode_t *)get_HashMap(closedSet, neighbours[i].location);

         if (neighbourNode == NULL || pathEstimate < neighbourNode->estimatedPathCost) {
            // we haven't been here, or if we have this one's better

            // create a new node, save some info to it
            neighbourNode = malloc(sizeof(pathNode_t));
            assert(neighbourNode != NULL);

            neighbourNode->location = neighbours[i].location;
            neighbourNode->cameFrom = currentNode;
            neighbourNode->stepsToStart = currentNode->stepsToStart + 1;
            neighbourNode->costToStart = costToStart;
            neighbourNode->estimatedPathCost = pathEstimate;

            // keep track of all nodes we've created
            append_List(pathNodes, (any_t)neighbourNode);

            // add it to the open set, to explore further
            add_PQueue(openSet, (pq_value_t)neighbourNode, -pathEstimate);
         }
      }

      // don't need the neighbours anymore
      free(neighbours);
   }

   // clean up all nodes we created
   while (size_List(pathNodes) > 0) {
      currentNode = (pathNode_t *)removeFirst_List(pathNodes);
      free(currentNode);
   }
   destroy_List(pathNodes);
   destroy_HashMap(closedSet);
   destroy_PQueue(openSet);

   return pathLength;
}

// backtrack to build a path array
static int buildPath(pathNode_t *goalNode, path_t *path) {
   int pathLength;
   int i;
   pathNode_t *node = goalNode;


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
