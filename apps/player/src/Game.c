#include <stdlib.h>
#include <assert.h>

#include <stdio.h>

#include <SDL/SDL.h>
#include "core/Game.h"
#include "state/layer.h"

struct _game {
   struct layer *layer;
};

Game new_Game(char *title) {
	Game game = malloc(sizeof(struct _game));
	assert(game != NULL);

	printf("* [Initializing Map]\n");
	game->layer = layer_init("media/layers/layer_test", 0);

	if (game->layer == NULL) {
		printf("* [Initializing Map] Failed.\n");
		return NULL;
	}

	strncpy(title, "Alcubierre", TITLE_LENGTH);

	return game;
}

void destroy_Game(Game game) {
	free(game);
}

bool update_Game(Game game, Uint32 dt, Input input) {
	bool needsRedraw = true;
	mouse_t mouse = mouse_Input(input);

	if (mouse.leftPressed) {
		printf("Mouse Pressed\n");
	}
	if (mouse.leftDown) {
		printf("Mouse Down at (%d, %d)\n", mouse.x, mouse.y);
	}
	if (mouse.leftReleased) {
		printf("Mouse Released\n");
	}

	if (isPressed_Input(input, SDLK_b)) {
		printf("b Pressed\n");
	}
	if (isDown_Input(input, SDLK_b)) {
		printf("b Down\n");
	}
	if (isReleased_Input(input, SDLK_b)) {
		printf("b Released\n");
	}

	return needsRedraw;
}

void draw_Game(Game game, SDL_Surface *screen) {
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format,0,0,0));
}
