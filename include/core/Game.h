#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL/SDL.h>
#include "io/Input.h"

#define TITLE_LENGTH 128

typedef struct _game *Game;

Game new_Game(char *title);
void destroy_Game(Game game);

bool update_Game(Game game, Uint32 dt, Input input);
void draw_Game(Game game, SDL_Surface *screen);

#endif
