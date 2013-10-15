#include "types/any.h"
#include "types/PQueue.h"
#include "types/HashMap.h"
#include "types/List.h"
#include "pathfinding/aStar.h"

#define MAP_SIZE 1279

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct pathNode pathNode_t;

typedef struct {
   location_t cameFrom;
   int fScore;
   int gScore;
   int step;
} locationData_t;

static int buildPath(location_t goalNode, HashMap cameFrom, path_t *path);

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
   anyEqDelegate isEqual,
   
   void *data
) {
   PQueue openPriorities;

   HashMap openSet;
   HashMap closedSet;
   
   HashMap locationDataMap;

   List allocatedData;

   expansion_t *neighbours;
   expansion_t expansion;

   int numNeighbours;
   int i;

   int tentativeG;
   int tentativeF;

   bool isInOpenSet;

   location_t current;
   location_t neighbour;

   int pathLength = NO_PATH;

   locationData_t *currentData;
   locationData_t *neighbourData;



   // hashmap of f and g scores for each location, and where they came from
   locationDataMap = new_HashMap(hashFunction, isEqual, MAP_SIZE);

   // list of all allocated location data
   allocatedData = new_List();


   // a priority queue of unexplored nodes, ranked by cost
   // and a hashmap of the frontier nodes
   openPriorities = new_PQueue();
   openSet = new_HashMap(hashFunction, isEqual, MAP_SIZE);

   // a hashmap of the best expanded nodes
   closedSet = new_HashMap(hashFunction, isEqual, MAP_SIZE);


   // initialise start positions g and f scores
   currentData = malloc(sizeof(locationData_t));
   assert(currentData != NULL);
   append_List(allocatedData, (any_t)currentData);

   currentData->cameFrom = (location_t)NULL;
   currentData->gScore = 0;
   currentData->fScore = currentData->gScore + heuristic(start, data);
   currentData->step = 0;

   set_HashMap(locationDataMap, (any_t)start, (any_t)currentData);


   // add the first node to the open set
   add_PQueue(openPriorities, (any_t)start, -(currentData->fScore));
   set_HashMap(openSet, (any_t)start, (any_t)true);

   while (size_PQueue(openPriorities) > 0) {
      // remove the node on the best path so far, thought to be closest to the goal
      current = (location_t)remove_PQueue(openPriorities);
      remove_HashMap(openSet, (any_t)current);

      // check if it's a goal, if so build the path
      if (goalCondition(current, data)) {
         pathLength = buildPath(current, locationDataMap, path);
         break;
      }

      // mark this node as visited, saving it to the closed set, keyed by the location
      set_HashMap(closedSet, (any_t)current, (any_t)true);

      // retrieve data for current node
      currentData = (locationData_t *)get_HashMap(locationDataMap, (any_t)current);

      // expand this node and find its neighbours
      numNeighbours = expander(current, &neighbours, data);

      for (i = 0; i != numNeighbours; ++i) {
         expansion = neighbours[i];
         neighbour = expansion.location;

         // retrieve data for neighbour, if it exists
         neighbourData = (locationData_t *)get_HashMap(locationDataMap, (any_t)neighbour);

         // for each neighbour determine the cost so far and estimated total
         // path cost
         tentativeG = currentData->gScore + expansion.cost;
         tentativeF = tentativeG + heuristic(neighbour, data);

         if (in_HashMap(closedSet, (any_t)neighbour) && tentativeF >= neighbourData->fScore) {
            // already expanded (and last time was better)
            continue;
         }

         isInOpenSet = in_HashMap(openSet, (any_t)neighbour);

         if (!isInOpenSet || tentativeF < neighbourData->fScore) {
            // we haven't added this node yet, or if we did, it wasn't as good

            // allocate memory for this neighbour's data if none exists already
            if (neighbourData == NULL) {
               neighbourData = malloc(sizeof(locationData_t));
               assert(neighbourData != NULL);
               append_List(allocatedData, (any_t)neighbourData);
            }

            // store the info for this expanded neighbour
            neighbourData->cameFrom = current;
            neighbourData->gScore = tentativeG;
            neighbourData->fScore = tentativeF;
            neighbourData->step = currentData->step + 1;

            set_HashMap(locationDataMap, (any_t)neighbour, (any_t)neighbourData);

            if (!isInOpenSet) {
               // add it to the open set, to explore further
               add_PQueue(openPriorities, (any_t)neighbour, -tentativeF);
               set_HashMap(openSet, (any_t)neighbour, (any_t)true);
            }
         }
      }

      // don't need the neighbours anymore
      free(neighbours);
   }

   destroy_HashMap(openSet);
   destroy_PQueue(openPriorities);
   destroy_HashMap(closedSet);
   destroy_HashMap(locationDataMap);

   // free all location data
   while (size_List(allocatedData) != 0) {
      currentData = (locationData_t *)removeFirst_List(allocatedData);
      free(currentData);
   }
   destroy_List(allocatedData);

   return pathLength;
}

// backtrack to build a path array
static int buildPath(location_t goalNode, HashMap locationDataMap, path_t *path) {
   int pathLength;
   int i;

   location_t current;
   locationData_t *locationData;

   current = goalNode;
   locationData = (locationData_t *)get_HashMap(locationDataMap, (any_t)current);

   pathLength = locationData->step + 1;
   *path = malloc(sizeof(location_t) * pathLength);
   assert(*path != NULL);

   i = 0;
   while (locationData->cameFrom != (location_t)NULL) {
      (*path)[pathLength-i-1] = current;
      
      current = locationData->cameFrom;
      locationData = (locationData_t *)get_HashMap(locationDataMap, (any_t)current);
      assert(locationData != NULL);
      ++i;
   }
   (*path)[pathLength-i-1] = current;

   return pathLength;
}
