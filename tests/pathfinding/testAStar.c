#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "pathfinding/aStar.h"

typedef struct {
	int x;
	int y;
	bool occupied;
} coord_t;

typedef struct {
	coord_t grid[16][16];
	coord_t start;
	coord_t end;
} maze_t;

bool isEqualCoord(location_t a, location_t b) {
	coord_t *coordA = (coord_t *)a;
	coord_t *coordB = (coord_t *)b;

	return (coordA->x == coordB->x && coordA->y == coordB->y);
}

bool isGoal(location_t location, void *data) {
	maze_t *maze = (maze_t *)data;
	coord_t *goal  = &(maze->end);

	return isEqualCoord(location, (location_t)goal);
}

int expandCoord(location_t location, expansion_t *neighbours, void *data) {
	maze_t *maze = (maze_t *)data;
	coord_t *coord = (coord_t *)location;
	int i, x, y;
	int numNeighbours = 0;
	coord_t moves[] = {
		{-1,  1, false},
		{ 1, -1, false},
		{ 1,  1, false},
		{-1, -1, false},
		{-1,  0, false},
		{ 1,  0, false},
		{ 0, -1, false},
		{ 0,  1, false}
	};

	*neighbours = malloc(sizeof(expansion_t *) * 8);
	assert(*neighbours != NULL);

	for (i = 0; i != 8; ++i) {
		x = coord->x + moves[i]->x;
		y = coord->y + moves[i]->y;
		if ((maze->grid[x][y]).occupied) {
			(*neighbours)[numNeighbours].cost = 1;
			(*neighbours)[numNeighbours].location = (location_t)&(maze->grid[x][y]);
			numNeighbours++;
		}
	}

	return numNeighbours;
}

int coordHeuristic(location_t location, void *data) {
	return 0;
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
	
}

int main(int argc, char *argv[]) {
	maze_t maze;

	maze.start.x = 1;
	maze.start.y = 1;

	maze.end.x = 15;
	maze.end.y = 15;

	path_t path;
	int pathLength;
	int i;

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

	for (i = 0; i != pathLength; ++i) {
		printCoord(path[i]);
	}

	return EXIT_SUCCESS;
}