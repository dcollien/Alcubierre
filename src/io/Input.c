#include <assert.h>


#include "io/Input.h"

#include "types/List.h"
#include "types/HashMap.h"

#define INPUT_MOUSE_LEFT (-1)
#define INPUT_MOUSE_RIGHT (-2)

#define KEYMAP_SIZE 283

struct _input {
   HashMap down;
   HashMap pressed;
   List released;
   mouse_t mouse;
};

static unsigned int keymapHash(any_t value);

Input new_Input(void) {
   Input input = malloc(sizeof(struct _input));
   assert(input != NULL);

   input->released = new_List();
   input->pressed = new_HashMap(keymapHash, NULL, KEYMAP_SIZE);
   input->down = new_HashMap(keymapHash, NULL, KEYMAP_SIZE);

   input->mouse.leftDown = false;
   input->mouse.rightDown = false;
   input->mouse.middleDown = false;

   input->mouse.leftReleased = false;
   input->mouse.rightReleased = false;
   input->mouse.middleReleased = false;

   input->mouse.leftPressed = false;
   input->mouse.rightPressed = false;
   input->mouse.middlePressed = false;

   input->mouse.x = 0;
   input->mouse.y = 0;

   return input;
}

void destroy_Input(Input input) {
   destroy_List(input->released);
   destroy_HashMap(input->pressed);
   destroy_HashMap(input->down);
}

bool isDown_Input(Input input, SDLKey key) {
   return ((bool)get_HashMap(input->down, (any_t)key) == true);
}

bool isPressed_Input(Input input, SDLKey key) {
   return ((bool)get_HashMap(input->pressed, (any_t)key) == true);
}

bool isReleased_Input(Input input, SDLKey key) {
   return in_List(input->released, (any_t)key, NULL);
}

void clearPressed_Input(Input input) {
   SDLKey key;


   while (size_List(input->released) != 0) {
      key = (SDLKey)removeFirst_List(input->released);
      set_HashMap(input->down, (any_t)key, (any_t)false);
   }

   empty_HashMap(input->pressed);

   // clear mouse events

   if (input->mouse.leftReleased) {
      input->mouse.leftDown = false;
   }
   input->mouse.leftReleased = false;

   if (input->mouse.rightReleased) {
      input->mouse.rightDown = false;
   }
   input->mouse.rightReleased = false;

   if (input->mouse.middleReleased) {
      input->mouse.middleDown = false;
   }
   input->mouse.middleReleased = false;

   input->mouse.leftPressed = false;
   input->mouse.rightPressed = false;
   input->mouse.middlePressed = false;
}

bool pollEvents_Input(Input input) {
   SDL_Event event;
   SDLKey key;

   while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) { 
      
         return false;
      
      } else if (event.type == SDL_MOUSEMOTION) {

         input->mouse.x = event.motion.x;
         input->mouse.y = event.motion.y;

      } else if (event.type == SDL_MOUSEBUTTONDOWN) {

         // Store mouse down events
         if (event.button.button == SDL_BUTTON_LEFT) {
            if (!input->mouse.leftDown) {
               input->mouse.leftPressed = true;
            }

            input->mouse.leftDown = true;
         } else if (event.button.button == SDL_BUTTON_RIGHT) {
            if (!input->mouse.rightDown) {
               input->mouse.rightPressed = true;
            }

            input->mouse.rightDown = true;
         } else if (event.button.button == SDL_BUTTON_MIDDLE) {
            if (!input->mouse.middleDown) {
               input->mouse.middlePressed = true;
            }

            input->mouse.middleDown = true;
         }

      } else if (event.type == SDL_MOUSEBUTTONUP) {

         // store mouse up events
         if (event.button.button == SDL_BUTTON_LEFT) {
            input->mouse.leftReleased = true;
         } else if (event.button.button == SDL_BUTTON_RIGHT) {
            input->mouse.rightReleased = true;
         } else if (event.button.button == SDL_BUTTON_MIDDLE) {
            input->mouse.middleReleased = true;
         }

      } else if (event.type == SDL_KEYDOWN) {

         // store key down events
         key = event.key.keysym.sym;

         if (!in_HashMap(input->down, (any_t)key)) {
            set_HashMap(input->pressed, (any_t)key, (any_t)true);
         }
         set_HashMap(input->down, (any_t)key, (any_t)true);

      } else if (event.type == SDL_KEYUP) {

         // store key up events
         key = event.key.keysym.sym;
         append_List(input->released, (any_t)key);

      }
   }

   return true;
}

mouse_t mouse_Input(Input input) {
   return input->mouse;
}

static unsigned int keymapHash(any_t value) {
   return (unsigned int)value;
}



