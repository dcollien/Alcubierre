typedef int sprite_t; // TODO

typedef int tileType_t;

#define TILE_EMPTY 0
#define TILE_OBSTACLE 1
#define TILE_HIDDEN 2


// at relative coord x, y:
// tileTypes[x*width+y] = empty/obstacle/hidden
// sprites[x*width+y] = tile to display
typedef struct {
	tileType_t *tileTypes;
	sprite_t *sprites;
	int width;
	int height;
} entity_t;
