#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL/SDL.h>

typedef struct _game *Game;

Game new_Game(void);
void destroy_Game(Game game);

bool update_Game(Game game, Uint32 dt);
void draw_Game(Game game, SDL_Surface *screen);

#endif
