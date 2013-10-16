#include "state/World.h"
#include "state/layer.h"
#include "core/types.h"

#define NUM_LAYERS 2

struct world {
    struct layer *layers[NUM_LAYERS];
};

static error_t world_parse_file(const char *file_path, World *world);

World *create_World(const char *file_path) {
    error_t err;
    World *new_world = NULL;
   
    printf("* [create_World][start]\n");
    new_world = malloc(sizeof(struct world));
    err = world_parse_file(file_path, new_world);

    if (err) {
        printf("* [create_World][failed]\n");
        destroy_World(new_world);
        return NULL;
    }

    printf("* [create_World][finished]\n");
    return new_world;
}
error_t world_parse_file(const char *file_path, World *world) {
    FILE *fin;
    ssize_t read;
    size_t len = 0;
    char *line = NULL;
    size_t cur_line = 0;
    
    fin = fopen(file_path, "r");
    if (fin == NULL) {
        return EXIT_FAILURE;
    }

    while ((read = getline(&line, &len, fin)) != -1) {
        if (cur_line < NUM_LAYERS) {
            line[strlen(line)-1] = '\0';
            world->layers[cur_line] = layer_init(line, 0);
        }
        cur_line++;
    }
    return 0;
}

void destroy_World(World *to_destroy) {
    int i;
    for (i = 0; i < NUM_LAYERS; i++) {
        layer_destroy(to_destroy->layers[i]);
    }
    free(to_destroy);
}

void render_World(SDL_Surface *screen, World *to_render) {
    int i;
    for (i = 0; i < NUM_LAYERS; i++) {
        layer_render(screen, to_render->layers[i]);
    }
}

void set_tile_World(World *to_set, int layer, int x, int y, int id) {
    to_set->layers[layer]->tiles[x + y*32] = id;
}
