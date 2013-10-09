#include <stdio.h>
#include <stdlib.h>

#include "limits.h"
#include "layer.h"

int main(int argc, char *argv[]) {
   struct layer *layer;
   size_t i;

   printf("* [Initializing Map]\n");
   layer = layer_init("media/layers/layer_test", 0);

   if (layer == NULL) {
        printf("* [Initializing Map] Failed.\n");
        return EXIT_FAILURE;
   }
   
   for (i = 0; i < MAP_SZ; i++) {
        if (i % 32 == 0 && i) {
            printf("\n");
        }
        printf("%d ", layer->tiles[i]);
   }

   return EXIT_SUCCESS;
}
