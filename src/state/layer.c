/* @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @author David Collien
 * @description A layer is a portion of a map that is rendered, we will be
 *  using painters algorithm to stich multiple layers into a map.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "core/limits.h"
#include "state/layer.h"
#include <SDL/SDL.h>


static error_t layer_parse_file(const char *path, struct layer *layer);

struct layer *layer_init(const char *path, flags_t flags) {
    error_t err;
    struct layer *new_layer;

    new_layer = malloc(sizeof(struct layer));
    err = layer_parse_file(path, new_layer);
    if (err) {
        free(new_layer);
        return NULL;
    }
    return new_layer;
}

void layer_destroy(struct layer *to_destroy) {
    free(to_destroy);
}

error_t layer_parse_file(const char *path, struct layer *layer) {
    FILE *fin;
    ssize_t read;
    size_t len = 0;
    char *line = NULL;
    size_t cur_line = 0;
    size_t offset = 0;
    char *tok;
    
    fin = fopen(path, "r");
    if (fin == NULL) {
        return EXIT_FAILURE;
    }

    while ((read = getline(&line, &len, fin)) != -1) {
        if (cur_line == 0) {
            line[strlen(line)-1] = '\0';
            layer->tilesheet = create_Sprite(line);
        } else if (cur_line < (MAP_COLS + 1)) {
            printf("%lu: %s", cur_line - 1, line);
            tok = strtok(line, " ,");
            while (tok != NULL) {
                layer->tiles[offset] = atoi(tok);
                offset++;
                tok = strtok(NULL, " ,");
            }
        }
        cur_line++;
    }
    return 0;
}

void layer_render(SDL_Surface *screen, struct layer *layer) {
    int i, j, offset;
    for (i = 0; i < MAP_COLS; i++) {
        for (j = 0; j < MAP_ROWS; j++) {
            offset = layer->tiles[i*32+j];
            if (offset >= 0) {
                frame_Sprite(layer->tilesheet, offset, 0);
                position_Sprite(layer->tilesheet, j * 32, i * 32);
                render_Sprite(screen, layer->tilesheet);
            }
        }
    }
}
