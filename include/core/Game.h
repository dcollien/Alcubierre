#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL/SDL.h>
#include "io/Input.h"

#define TITLE_LENGTH 128

typedef struct _game *Game;

typedef struct {
	int width;
	int height;
	char title[TITLE_LENGTH];
} windowInfo_t;

Game new_Game(void);
void destroy_Game(Game game);

windowInfo_t getWindow_Game(Game game);

bool update_Game(Game game, Uint32 dt, Input input);
void draw_Game(Game game, SDL_Surface *screen);

#endif
