#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <SDL/SDL.h>

typedef struct _input *Input;

typedef struct {
	int x;
	int y;

	bool leftDown;
	bool middleDown;
	bool rightDown;
	
	bool leftPressed;
	bool middlePressed;
	bool rightPressed;

	bool leftReleased;
	bool middleReleased;
	bool rightReleased;
} mouse_t;

Input new_Input(void);

void destroy_Input(Input input);

bool isDown_Input(Input input, SDLKey key);
bool isPressed_Input(Input input, SDLKey key);
bool isReleased_Input(Input input, SDLKey key);

mouse_t mouse_Input(Input input);

bool pollEvents_Input(Input input);
void clearPressed_Input(Input input);

#endif
