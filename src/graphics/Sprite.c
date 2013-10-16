#include "graphics/Sprite.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>

struct sprite {
    SDL_Surface *image;
};

Sprite *create_Sprite(char *file_path) {
    struct sprite *new_sprite = NULL;
    SDL_Surface *loaded_image = NULL;

    loaded_image = IMG_Load(file_path);
    // Image failed to load.
    if (loaded_image == NULL) {
        return NULL;
    }

    new_sprite = malloc(sizeof(struct sprite));
    new_sprite->image = SDL_DisplayFormat(loaded_image);
    SDL_FreeSurface(loaded_image);

    return new_sprite;
}

void destroy_Sprite(Sprite *to_destroy) {
    SDL_FreeSurface(to_destroy->image);
    free(to_destroy);
}

void render_Sprite(SDL_Surface *window, Sprite *to_render) {
    SDL_BlitSurface(to_render->image, NULL, window, NULL);
    SDL_Flip(window);
}
