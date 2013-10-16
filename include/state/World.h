#ifndef ALC_WORLD_H_
#define ALC_WORLD_H_

#include <SDL/SDL.h>

typedef struct world World;

World *create_World(const char *file_path);

void destroy_World(World *to_destroy);

void render_World(SDL_Surface *screen, World *to_render);

void set_tile_World(World *to_set, int layer, int x, int y, int id);

#endif /* ALC_WORLD_H_ */
