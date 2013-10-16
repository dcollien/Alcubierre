#ifndef ALC_SPRITE_H_
#define ALC_SPRITE_H_

#include <SDL/SDL.h>

typedef struct sprite Sprite;

Sprite *create_Sprite(char *file_path);

void destroy_Sprite(Sprite *to_destroy);

void position_Sprite(Sprite *to_position, int x, int y);

// TODO make a window.
void render_Sprite(SDL_Surface *window, Sprite *to_render);

#endif /* ALC_SPRITE_H_ */
