#ifndef ALC_GRAPHICS_WINDOW_H_
#define ALC_GRAPHICS_WINDOW_H_

#include <stddef.h>

typedef struct window Window;

Window *create_Window(size_t width, size_t height, const char *title);

void render_Window(Window *to_render);

void destroy_Window(Window *to_destroy);

#endif /* ALC_GRAPHICS_WINDOW_H_ */
