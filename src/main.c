#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <SDL/SDL.h>

#include "limits.h"

#include "game/Game.h"

#define WINDOW_TITLE "Alcubierre"

static SDL_Surface *screen_create(const char *title, size_t width,
   size_t height);

int main(int argc, char *argv[]) {
   SDL_Surface *screen;
   SDL_Event event;
   Game game;

   int running;

   Uint32 lastUpdate;
   Uint32 thisUpdate;
   Uint32 dt;
   //size_t i;


   game = new_Game();
   assert(game != NULL);

   running = true;
   
   screen = screen_create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
   while (running) {
      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_QUIT) { 
            running = false;
            break;
         }
      }

      thisUpdate = SDL_GetTicks();
      dt = thisUpdate - lastUpdate;
      lastUpdate = thisUpdate;

      if (update_Game(game, dt)) {
         draw_Game(game, screen);
      }

      SDL_Flip(screen);
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
