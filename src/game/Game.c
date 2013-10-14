#include <stdlib.h>
#include <assert.h>

#include <SDL/SDL.h>
#include "game/Game.h"
#include "game/layer.h"

struct _game {
   struct layer *layer;
};

Game new_Game(void) {
	Game game = malloc(sizeof(struct _game));
	assert(game != NULL);

	printf("* [Initializing Map]\n");
	game->layer = layer_init("media/layers/layer_test", 0);

	if (game->layer == NULL) {
		printf("* [Initializing Map] Failed.\n");
		return NULL;
	}

	return game;
}

void destroy_Game(Game game) {
	free(game);
}

bool update_Game(Game game, Uint32 dt) {
	bool needsRedraw = true;

	return needsRedraw;
}

void draw_Game(Game game, SDL_Surface *screen) {
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format,0,0,0));
}
