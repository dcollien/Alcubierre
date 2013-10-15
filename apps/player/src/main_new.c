#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <SDL/SDL.h>

#include "limits.h"

#include "game/Game.h"
#include "game/Input.h"

#define WINDOW_TITLE "Alcubierre"

static SDL_Surface *screen_create(
   const char *title, 
   size_t width,
   size_t height
);

int main(int argc, char *argv[]) {
   SDL_Surface *screen;

   Input input;
   Game game;

   bool isRunning;

   Uint32 lastUpdate;
   Uint32 thisUpdate;
   Uint32 dt;
   //size_t i;

   input = new_Input();
   game = new_Game();
   assert(game != NULL);

   isRunning = true;
   
   screen = screen_create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
   while (isRunning) {
      if (!pollEvents_Input(input)) {
         isRunning = false;
      }

      thisUpdate = SDL_GetTicks();
      dt = thisUpdate - lastUpdate;
      lastUpdate = thisUpdate;

      if (update_Game(game, dt, input)) {
         draw_Game(game, screen);
      }
      
      SDL_Flip(screen);
      
      clearPressed_Input(input);
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
