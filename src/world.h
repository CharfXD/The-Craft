// Simple voxel world
#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

#define WORLD_X 16
#define WORLD_Y 8
#define WORLD_Z 16

typedef uint8_t block_t;

typedef struct {
    block_t blocks[WORLD_X][WORLD_Y][WORLD_Z];
} World;

void world_init(World *w);
block_t world_get(World *w, int x, int y, int z);
void world_set(World *w, int x, int y, int z, block_t b);

#endif // WORLD_H
