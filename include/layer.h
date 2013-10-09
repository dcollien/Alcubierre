/* @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 * @author David Collien <dcollien@cse.unsw.edu.au>
 * @description A layer is a portion of a map that is rendered, we will be
 *  using painters algorithm to stich multiple layers into a map.
 */
#ifndef ALC_LAYER_H_
#define ALC_LAYER_H_

#include "types.h"

/* Prototype */
struct layer;

/**
 * @brief Constructs a layer from the file path returning null if the file
 *        does not exist.
 * @param path the path of the file to retrieve
 * @param flags optional flags, non currently avaliable.
 * @return returns a pointer to the layer, if it fails it returns NULL.
 */
struct layer *layer_init(const char *path, flags_t flags);

/**
 * @brief  Takes a layer and attempts to destroy it.
 * @param to_destroy the layer you wish to destroy.
 */
void layer_destroy(struct layer *to_destroy);

/**
 * @brief Renders the layer 
 * @param to_render the layer you wish to render.
 * @return an error if something went wrong.
 */
error_t layer_render(struct layer *to_render);

#endif /* ALC_LAYER_H_ */
