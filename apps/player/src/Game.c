#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <stdio.h>

#include <SDL/SDL.h>
#include "core/Game.h"
#include "state/layer.h"

#include "core/vector2d.h"
#include "graphics/Sprite.h"

#include "window.h"

#define CURSOR_SIZE 10

struct _game {
   struct layer *layer;
   vector2d_t cursor;
   bool isDragging;
   Sprite *test_sprite;
};

Game new_Game(void) {
	Game game = malloc(sizeof(struct _game));
	assert(game != NULL);

	printf("* [Initializing Map]\n");
	game->layer = layer_init("../media/layers/layer_test", 0);

	if (game->layer == NULL) {
		printf("* [Initializing Map] Failed.\n");
		return NULL;
	}

	game->cursor = v_(0,0);
	game->isDragging = false;
    game->test_sprite = create_Sprite("../media/objects/toilet.png");
    assert(game->test_sprite != NULL);

	return game;
}

windowInfo_t getWindow_Game(Game game) {
	windowInfo_t window;

	window.width = WINDOW_WIDTH;
	window.height = WINDOW_HEIGHT;

	strncpy(window.title, WINDOW_TITLE, TITLE_LENGTH);

	return window;
}

void destroy_Game(Game game) {
    destroy_Sprite(game->test_sprite);
	free(game);
}

bool update_Game(Game game, Uint32 dt, Input input) {
	bool needsRedraw = true;
	mouse_t mouse = mouse_Input(input);

	game->cursor.x = mouse.x;
	game->cursor.y = mouse.y;

	if (mouse.leftPressed) {
		printf("Mouse Pressed\n");
		game->isDragging = true;
	}
	if (mouse.leftDown) {
		printf("Mouse Down at (%d, %d)\n", mouse.x, mouse.y);
	}
	if (mouse.leftReleased) {
		printf("Mouse Released\n");
		game->isDragging = false;
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

	SDL_Rect cursor = {
		game->cursor.x - CURSOR_SIZE/2,
		game->cursor.y - CURSOR_SIZE/2,
		CURSOR_SIZE,
		CURSOR_SIZE
	};

	if (game->isDragging) {
		SDL_FillRect(screen, &cursor, SDL_MapRGB(screen->format,255,0,0));	
	} else {
		SDL_FillRect(screen, &cursor, SDL_MapRGB(screen->format,0,255,0));	
	}
    assert(game->test_sprite);
    assert(screen);
    position_Sprite(game->test_sprite, game->cursor.x, game->cursor.y);
    render_Sprite(screen, game->test_sprite);
}
