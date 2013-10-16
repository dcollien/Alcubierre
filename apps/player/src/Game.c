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
   int frame, last_x, last_y;
};

Game new_Game(void) {
	Game game = malloc(sizeof(struct _game));
	assert(game != NULL);

	printf("* [Initializing Map]\n");
	game->layer = layer_init("../media/layers/map", 0);

	if (game->layer == NULL) {
		printf("* [Initializing Map] Failed.\n");
		return NULL;
	}

    game->frame = 0;
	game->cursor = v_(0,0);
	game->isDragging = false;
    game->test_sprite = create_Sprite("../media/sprite.png");
    game->last_x = 10;
    game->last_y = 10;

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

 
    layer_render(screen, game->layer);

    game->frame = (game->frame + 1) %  7;
    frame_Sprite(game->test_sprite, game->frame , 0);
    
    if (game->isDragging) {
        game->last_x = ((int)game->cursor.x/32) * 32;
        game->last_y = ((int)game->cursor.y/32) * 32;
    }
    vector2d_t v = get_position_Sprite(game->test_sprite);
    if ((int)v.x != game->last_x) v.x = v.x < game->last_x ? (v.x + 1) : (v.x - 1);
    if ((int)v.y != game->last_y) v.y = v.y < game->last_y ? (v.y + 1) : (v.y - 1);
    position_Sprite(game->test_sprite, v.x, v.y);

    render_Sprite(screen, game->test_sprite);

}
