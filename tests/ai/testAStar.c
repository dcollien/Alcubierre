#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "ai/aStar.h"


#define TYPE_OPEN 0
#define TYPE_OCCUPIED 1
#define TYPE_PATH 2

typedef struct {
   int x;
   int y;
   int type;
} coord_t;

typedef struct {
   coord_t grid[16][16];
   coord_t start;
   coord_t end;
} maze_t;

bool isEqualCoord(location_t locationA, location_t locationB) {
   coord_t *a = (coord_t *)locationA;
   coord_t *b = (coord_t *)locationB;

   return (a->x == b->x && a->y == b->y);
}

bool isGoal(location_t location, void *data) {
   maze_t *maze = (maze_t *)data;
   coord_t *coord = (coord_t *)location;
   coord_t *goal  = &(maze->end);
   bool isSameCoord = isEqualCoord((location_t)coord, (location_t)goal);

   if (isSameCoord) {
      printf("GOAL FOUND!\n");
   }

   return isSameCoord;
}

int expandCoord(location_t location, expansion_t **neighbours, void *data) {
   maze_t *maze = (maze_t *)data;
   coord_t *coord = (coord_t *)location;
   int i, x, y;
   int numNeighbours = 0;
   coord_t moves[] = {
      {-1, -1, false}, { 0, -1, false}, { 1, -1, false},
      {-1,  0, false},                  { 1,  0, false},
      {-1,  1, false}, { 0,  1, false}, { 1,  1, false}
   };

   *neighbours = malloc(sizeof(expansion_t) * 8);
   assert(*neighbours != NULL);

   //printf("Expanding (%d, %d)\n", coord->x, coord->y);
   for (i = 0; i != 8; ++i) {
      x = coord->x + moves[i].x;
      y = coord->y + moves[i].y;

      if (x < 0 || x >= 16) {
         continue;
      }
      if (y < 0 || y >= 16) {
         continue;
      }

      if ((maze->grid[x][y]).type != TYPE_OCCUPIED) {
         if (i == 0 || i == 2 || i == 5 || i == 7) {
            // diagonals
            (*neighbours)[numNeighbours].cost = 1414;
         } else {
            (*neighbours)[numNeighbours].cost = 1000;
         }
         (*neighbours)[numNeighbours].location = (location_t)&(maze->grid[x][y]);

         numNeighbours++;
      }
   }

   return numNeighbours;
}

int coordHeuristic(location_t location, void *data) {
   maze_t *maze = (maze_t *)data;
   coord_t *coord = (coord_t *)location;

   int dx = coord->x - maze->end.x;
   int dy = coord->y - maze->end.y;

   return (int)(sqrt(dx * dx + dy * dy) * 1000);
}

unsigned int coordHash(location_t location) {
   coord_t *coord = (coord_t *)location;

   return (coord->y * 16 + coord->x);
}

void printCoord(location_t location) {
   coord_t *coord = (coord_t *)location;
   printf("(%d, %d)\n", coord->x, coord->y);
}

void generateMaze(maze_t *maze) {
   int x, y;
   coord_t *coord;

   for (x = 0; x != 16; ++x){
      for (y = 0; y != 16; ++y) {
         coord = &(maze->grid[x][y]);
         coord->x = x;
         coord->y = y;

         if (x == 0 || x == 15 || y == 0 || y == 15) {
            coord->type = TYPE_OCCUPIED;
         } else if (y == 5 && x != 12) {
            coord->type = TYPE_OCCUPIED;
         } else if (y == 10 && x != 3) {
            coord->type = TYPE_OCCUPIED;
         } else if ((x == 7 || x == 8) && y != 2 && y != 8 && y != 14) {
            coord->type = TYPE_OCCUPIED;
         } else {
            coord->type = TYPE_OPEN;
         }
      }
   }

   maze->start.x = 1;
   maze->start.y = 1;
   maze->start.type = TYPE_OPEN;

   maze->end.x = 14;
   maze->end.y = 14;
   maze->end.type = TYPE_OPEN;
}

void drawPath(maze_t *maze, path_t path, int pathLength) {
   int i;
   coord_t *coord;

   for (i = 0; i != pathLength; ++i) {
      coord = (coord_t *)path[i];
      maze->grid[coord->x][coord->y].type = TYPE_PATH;
   }
}

void printMaze(maze_t *maze) {
   int x, y;
   coord_t *coord;
   
   for (y = 0; y != 16; ++y) {
      for (x = 0; x != 16; ++x) {
         coord = &(maze->grid[x][y]);

         if (coord->x == maze->start.x && coord->y == maze->start.y) {
            printf("[]");
         } else if (coord->x == maze->end.x && coord->y == maze->end.y) {
            printf("!!");
         } else if (coord->type == TYPE_OCCUPIED) {
            printf("##");
         } else if (coord->type == TYPE_PATH) {
            printf("()");
         } else {
            printf("  ");
         }
      }
      printf("\n");
   }
}

int main(int argc, char *argv[]) {
   maze_t maze;

   maze.start.x = 1;
   maze.start.y = 1;

   maze.end.x = 15;
   maze.end.y = 15;

   path_t path;
   int pathLength;

   generateMaze(&maze);

   pathLength = aStar_shortestPath(
      (location_t)&(maze.start),
      isGoal,
      &path,
      expandCoord,
      coordHeuristic,
      coordHash,
      isEqualCoord,
      (void *)&maze
   );

   if (pathLength == NO_PATH) {
      printf("No Path Found\n");
   } else {
      printf("Path Length: %d\n", pathLength);
   }

   drawPath(&maze, path, pathLength);

   printMaze(&maze);

   assert(pathLength == 32);

   printf("All Tests Passed\n");

   return EXIT_SUCCESS;
}