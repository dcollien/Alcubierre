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
#define MOV_RATE 1

struct _game {
   World *world;
   vector2d_t cursor;
   bool isDragging;
   Sprite *crew[5];
   int frame, last_x, last_y;
   int id, layer, crew_id;
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

    game->crew_id = 0;
    game->frame = 0;
	game->cursor = v_(0,0);
	game->isDragging = false;
    game->crew[0] = create_Sprite("../media/sprites/dudebro_blonde.png");
    game->crew[1] = create_Sprite("../media/sprites/golem_alien_blue.png");
    game->crew[2] = create_Sprite("../media/sprites/lizard_alien_purple.png");
    game->crew[3] = create_Sprite("../media/sprites/mysterious_alien.png");
    game->crew[4] = create_Sprite("../media/sprites/purple_alien_longhair.png");

    game->last_x = 10;
    game->last_y = 10;

    assert(game->crew[game->crew_id] != NULL);

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
    destroy_Sprite(game->crew[game->crew_id]);
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
		printf("Mouse Released dt: %d\n", dt);
		game->isDragging = false;
	}

	if (isPressed_Input(input, SDLK_b)) {
		printf("b Pressed\n");
	}
	if (isDown_Input(input, SDLK_b)) {
		printf("b Down\n");
	
    }


	if (isReleased_Input(input, SDLK_a)) {
        game->crew_id = (game->crew_id + 1) % 5;
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
    
    if (game->isDragging) {
        game->last_x = ((int)game->cursor.x/32) * 32;
        game->last_y = ((int)game->cursor.y/32) * 32;
    }

    vector2d_t v = get_position_Sprite(game->crew[game->crew_id]);
    if (((int)v.y != game->last_y) || ((int)v.x != game->last_x)) {
        game->frame = (game->frame + 1) %  3;
        if ((int)v.y > game->last_y) {
            v.y = v.y - MOV_RATE;
            frame_Sprite(game->crew[game->crew_id], game->frame , 3);
        } else if ((int)v.y < game->last_y) {
            v.y = v.y + MOV_RATE;
            frame_Sprite(game->crew[game->crew_id], game->frame , 0);
        }

        if ((int)v.x > game->last_x) {
            v.x = v.x - MOV_RATE;
            frame_Sprite(game->crew[game->crew_id], game->frame , 1);
        } else if ((int)v.x < game->last_x) {
            v.x = v.x + MOV_RATE;
            frame_Sprite(game->crew[game->crew_id], game->frame , 2);
        }
        position_Sprite(game->crew[game->crew_id], v.x, v.y);
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
    assert(game->crew[game->crew_id]);
    assert(screen);

    render_World(screen, game->world);
    int i;
    for (i = 0; i < 5; i++) {
        render_Sprite(screen, game->crew[i]);
    }

}
