#include "graphics/Sprite.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "core/vector2d.h"

struct sprite {
    SDL_Surface *image;
    SDL_Rect position;
    SDL_Rect frame;
    int flag;
};

Sprite *create_Sprite(char *file_path) {
    printf("* [create_sprite][start] file_path: %s \n", file_path);
    struct sprite *new_sprite = NULL;
    SDL_Surface *loaded_image = NULL;
    //int color_key;

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
    new_sprite->flag = 0;
    

    /*
    */

    new_sprite->frame.x = 0;
    new_sprite->frame.y = 0;
    new_sprite->frame.w = 32;
    new_sprite->frame.h = 32;

    printf("* [create_sprite][finished] file_path: %s \n", file_path);

    return new_sprite;
}

void destroy_Sprite(Sprite *to_destroy) {
    SDL_FreeSurface(to_destroy->image);
    free(to_destroy);
}

void frame_Sprite(Sprite *to_frame, int x, int y) {
    to_frame->frame.x = x * 32;
    to_frame->frame.y = y * 32;
}

vector2d_t get_position_Sprite(Sprite *to_get) {
    vector2d_t v;
    v.x = to_get->position.x;
    v.y = to_get->position.y;
    return v;
}

vector2d_t get_frame_Sprite(Sprite *to_get) {
    vector2d_t v;
    v.x = to_get->frame.x;
    v.y = to_get->frame.y;
    return v;
}


void position_Sprite(Sprite *to_position, int x, int y) {
    to_position->position.x = x;
    to_position->position.y = y;
}

void render_Sprite(SDL_Surface *window, Sprite *to_render) {
    if (!to_render->flag) {
        int color_key = SDL_MapRGB(window->format, 255, 0, 255);
        SDL_SetColorKey(to_render->image, SDL_SRCCOLORKEY | SDL_RLEACCEL, color_key);
        to_render->flag = 1;
    }


    SDL_BlitSurface(to_render->image, &to_render->frame, window, &to_render->position);
}
