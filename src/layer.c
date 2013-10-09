/* @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @author David Collien
 * @description A layer is a portion of a map that is rendered, we will be
 *  using painters algorithm to stich multiple layers into a map.
 */

#include <stdlib.h>
#include <stdio.h>
#include "limits.h"
#include "layer.h"

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

error_t layer_render(struct layer *to_render) {
    return EXIT_SUCCESS;
}

error_t layer_parse_file(const char *path, struct layer *layer) {
    size_t read_len;
    FILE *fin;
    
    fin = fopen(path, "rb");
    if (fin != NULL) {
        read_len = fread(layer->tiles, 1, MAP_SZ, fin);
        fclose(fin);
        if (read_len) {
            printf(" -> [layer_parse_file] Read %lu bytes\n", read_len);
            return EXIT_SUCCESS;        
        }
    }

    return EXIT_FAILURE;
}
