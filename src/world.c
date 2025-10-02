#include "world.h"
#include <string.h>

void world_init(World *w) {
    memset(w->blocks, 0, sizeof(w->blocks));
    // create a floor
    for (int x = 0; x < WORLD_X; ++x) {
        for (int z = 0; z < WORLD_Z; ++z) {
            w->blocks[x][0][z] = 1; // ground
        }
    }
    // add a small hill
    for (int x = 4; x < 10; ++x) {
        for (int z = 4; z < 10; ++z) {
            w->blocks[x][1][z] = 2;
            w->blocks[x][2][z] = 2;
        }
    }
}

block_t world_get(World *w, int x, int y, int z) {
    if (x < 0 || x >= WORLD_X || y < 0 || y >= WORLD_Y || z < 0 || z >= WORLD_Z) return 0;
    return w->blocks[x][y][z];
}

void world_set(World *w, int x, int y, int z, block_t b) {
    if (x < 0 || x >= WORLD_X || y < 0 || y >= WORLD_Y || z < 0 || z >= WORLD_Z) return;
    w->blocks[x][y][z] = b;
}
