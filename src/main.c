#include <stdio.h>
#include <stdlib.h>
#include "limits.h"
#include "layer.h"
#include "SDL.h"

static SDL_Surface *screen_create(const char *title, size_t width,
        size_t height);

int main(int argc, char *argv[]) {
   SDL_Surface *screen;
   SDL_Event event;

   int running;
   struct layer *layer;
   size_t i;

   running = 1;
   printf("* [Initializing Map]\n");
   layer = layer_init("media/layers/layer_test", 0);

   if (layer == NULL) {
        printf("* [Initializing Map] Failed.\n");
        return EXIT_FAILURE;
   }
   
   screen = screen_create("Alc", WINDOW_WIDTH, WINDOW_HEIGHT);
   while (running) {
       while (SDL_PollEvent(&event)) {
         if (event.type == SDL_QUIT) { 
             running = 0;
             break;
         }
         SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format,0,0,0));
         SDL_Flip(screen);
       }
   }

   SDL_Quit();

   return EXIT_SUCCESS;
}

SDL_Surface *screen_create(const char *title, size_t width, size_t height) {
    SDL_Surface *new_screen;
   
    SDL_Init(SDL_INIT_VIDEO);
    new_screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption(title, title);

    return new_screen;
}
