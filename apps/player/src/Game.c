#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <stdio.h>

#include <SDL/SDL.h>
#include "core/Game.h"

#include "core/vector2d.h"
#include "graphics/Sprite.h"
#include "window.h"
#include "state/World.h"

#define CURSOR_SIZE 10

struct _game {
   World *world;
   vector2d_t cursor;
   bool isDragging;
   Sprite *test_sprite;
   int frame, last_x, last_y;
   int id, layer;
};

Game new_Game(void) {
	Game game = malloc(sizeof(struct _game));
	assert(game != NULL);

	printf("* [Initializing Map]\n");
    game->world = create_World("../media/world");

	if (game->world == NULL) {
		printf("* [Initializing Map] Failed.\n");
		return NULL;
	}

    game->frame = 0;
	game->cursor = v_(0,0);
	game->isDragging = false;
    game->test_sprite = create_Sprite("../media/sprites/dudebro_blonde.png");
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

    if (mouse.rightDown) {
		printf("Mouse Right Down at (%d, %d)\n", mouse.x, mouse.y);
        int x = ((int)game->cursor.x/32);
        int y = ((int)game->cursor.y/32);
        set_tile_World(game->world, game->layer, x, y, game->id);
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

	if (isReleased_Input(input, SDLK_d)) {
		printf("b Released\n");
        if (game->layer < 1) game->layer++;
	}
	if (isReleased_Input(input, SDLK_h)) {
		printf("b Released\n");
        if (game->layer > 0) game->layer--;
	}
    // Testing Shit.
	if (isReleased_Input(input, SDLK_b)) {
		printf("b Released\n");
        if (game->id < 5) game->id++;
	}
	if (isReleased_Input(input, SDLK_m)) {
		printf("b Released\n");
        if (game->id > 0) game->id--;
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

    render_World(screen, game->world);

    
    if (game->isDragging) {
        game->last_x = ((int)game->cursor.x/32) * 32;
        game->last_y = ((int)game->cursor.y/32) * 32;
    }
    vector2d_t v = get_position_Sprite(game->test_sprite);
    if ((int)v.x != game->last_x) v.x = v.x < game->last_x ? (v.x + 1) : (v.x - 1);
    if ((int)v.y != game->last_y) v.y = v.y < game->last_y ? (v.y + 1) : (v.y - 1);

    if ((int)v.y != game->last_y || (int)v.x != game->last_x) {
        game->frame = (game->frame + 1) %  3;
        frame_Sprite(game->test_sprite, game->frame , 0);
    }

    position_Sprite(game->test_sprite, v.x, v.y);

    render_Sprite(screen, game->test_sprite);

}
