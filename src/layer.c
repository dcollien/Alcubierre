/* @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @author David Collien
 * @description A layer is a portion of a map that is rendered, we will be
 *  using painters algorithm to stich multiple layers into a map.
 */

#include <stdlib.h>
#include "layer.h"

struct layer {
};

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
    return 0;
}

error_t layer_parse_file(const char *path, struct layer *layer) {
    return 0;
}

#endif /* ALC_LAYER_H_ */
