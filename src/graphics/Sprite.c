#include "graphics/Sprite.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct sprite {
    SDL_Surface *image;
    SDL_Rect position;
};

Sprite *create_Sprite(char *file_path) {
    printf("* [create_sprite][start] file_path: %s \n", file_path);
    struct sprite *new_sprite = NULL;
    SDL_Surface *loaded_image = NULL;

    printf(" -> [create_sprite] Loading Image \n");
    loaded_image = IMG_Load(file_path);
    // Image failed to load.
    if (loaded_image == NULL) {
        printf(" -> [create_sprite] Loading Image Failed \n");
        return NULL;
    }

    new_sprite = malloc(sizeof(struct sprite));
    printf(" -> [create_sprite] Optimizing Image \n");
    //new_sprite->image = SDL_DisplayFormat(loaded_image);
    new_sprite->image = loaded_image;
    printf(" -> [create_sprite] Freeing loaded_image \n");
    //SDL_FreeSurface(loaded_image);
    assert(new_sprite);
    position_Sprite(new_sprite, 0,0);
    printf("* [create_sprite][finished] file_path: %s \n", file_path);

    return new_sprite;
}

void destroy_Sprite(Sprite *to_destroy) {
    SDL_FreeSurface(to_destroy->image);
    free(to_destroy);
}

void position_Sprite(Sprite *to_position, int x, int y) {
    to_position->position.x = x;
    to_position->position.y = y;
}

void render_Sprite(SDL_Surface *window, Sprite *to_render) {
    printf("Rendering Sprite\n");
    assert(to_render);
    assert(to_render->image);


    SDL_BlitSurface(to_render->image, NULL, window, &to_render->position);
}
