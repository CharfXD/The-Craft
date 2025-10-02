#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "world.h"

#define WINDOW_W 800
#define WINDOW_H 600

typedef struct {
    float x, y, z; // camera position
    float pitch, yaw;
} Camera;

static void draw_block(SDL_Renderer *r, int sx, int sy, int size, SDL_Color color) {
    SDL_Rect rect = {sx, sy - size, size, size};
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(r, &rect);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderDrawRect(r, &rect);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("The Craft - Mini", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        SDL_DestroyWindow(win);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    World world;
    world_init(&world);


    Camera cam = {WORLD_X/2.0f, 4.0f, WORLD_Z/2.0f, 0.0f, 0.0f};

    // enable relative mouse mode for look-around
    SDL_SetRelativeMouseMode(SDL_TRUE);

    int running = 1;
    Uint64 perf_freq = SDL_GetPerformanceFrequency();
    Uint64 last = SDL_GetPerformanceCounter();

    while (running) {
        // delta time in seconds
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - last) / (float)perf_freq;
        if (dt > 0.1f) dt = 0.1f; // clamp
        last = now;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) running = 0;
                else if (e.key.keysym.sym == SDLK_m) {
                    // toggle mouse capture
                    int mode = SDL_GetRelativeMouseMode();
                    SDL_SetRelativeMouseMode(!mode);
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                // update yaw/pitch from relative mouse motion
                float sensitivity = 0.0025f; // radians per pixel
                cam.yaw   += e.motion.xrel * sensitivity;
                cam.pitch += -e.motion.yrel * sensitivity;
                if (cam.pitch > 1.4f) cam.pitch = 1.4f;
                if (cam.pitch < -1.4f) cam.pitch = -1.4f;
            }
        }

        // movement (frame-rate independent) using keyboard state
        const Uint8 *ks = SDL_GetKeyboardState(NULL);
        float speed = 5.0f; // units per second
        float sx = 0.0f, sz = 0.0f;
        // forward/back relative to yaw (camera looks toward negative Z at yaw=0)
        float forward_x = sinf(cam.yaw);
        float forward_z = -cosf(cam.yaw);
        float right_x = cosf(cam.yaw);
        float right_z = sinf(cam.yaw);

        if (ks[SDL_SCANCODE_W]) { sx += forward_x; sz += forward_z; }
        if (ks[SDL_SCANCODE_S]) { sx -= forward_x; sz -= forward_z; }
        if (ks[SDL_SCANCODE_D]) { sx += right_x; sz += right_z; }
        if (ks[SDL_SCANCODE_A]) { sx -= right_x; sz -= right_z; }

        // vertical movement
        if (ks[SDL_SCANCODE_Q]) cam.y += speed * dt;
        if (ks[SDL_SCANCODE_E]) cam.y -= speed * dt;

        // normalize horizontal movement to avoid faster diagonal speed
        float len = sqrtf(sx*sx + sz*sz);
        if (len > 0.001f) {
            sx = sx / len;
            sz = sz / len;
            cam.x += sx * speed * dt;
            cam.z += sz * speed * dt;
        }

        // place/remove blocks: Space to place above camera, Backspace to remove below
        static int prev_space = 0, prev_bksp = 0;
        int space = ks[SDL_SCANCODE_SPACE];
        int bksp = ks[SDL_SCANCODE_BACKSPACE];
        if (space && !prev_space) {
            int bx = (int)floorf(cam.x);
            int by = (int)floorf(cam.y);
            int bz = (int)floorf(cam.z);
            if (by+1 < WORLD_Y) world_set(&world, bx, by+1, bz, 3);
        }
        if (bksp && !prev_bksp) {
            int bx = (int)floorf(cam.x);
            int by = (int)floorf(cam.y);
            int bz = (int)floorf(cam.z);
            if (by-1 >= 0) world_set(&world, bx, by-1, bz, 0);
        }
        prev_space = space;
        prev_bksp = bksp;

        // Clear
        SDL_SetRenderDrawColor(ren, 135, 206, 235, 255); // sky
        SDL_RenderClear(ren);

        // Simple painter: draw from back to front
        const int block_size = 24;
        for (int y = 0; y < WORLD_Y; ++y) {
            for (int z = 0; z < WORLD_Z; ++z) {
                for (int x = 0; x < WORLD_X; ++x) {
                    block_t b = world_get(&world, x, y, z);
                    if (!b) continue;
                    // project to screen (very simple)
                    int sx = (int)((x - cam.x) * block_size) + WINDOW_W/2;
                    int sy = (int)(((z - cam.z) * block_size) - (y - cam.y) * block_size) + WINDOW_H/2;

                    SDL_Color col;
                    if (b == 1) col = (SDL_Color){139,69,19,255}; // ground dirt
                    else if (b == 2) col = (SDL_Color){34,139,34,255}; // grass/hill
                    else col = (SDL_Color){169,169,169,255}; // stone/other

                    draw_block(ren, sx, sy, block_size-2, col);
                }
            }
        }

        // HUD: simple crosshair
        SDL_SetRenderDrawColor(ren, 0,0,0,255);
        SDL_RenderDrawLine(ren, WINDOW_W/2-10, WINDOW_H/2, WINDOW_W/2+10, WINDOW_H/2);
        SDL_RenderDrawLine(ren, WINDOW_W/2, WINDOW_H/2-10, WINDOW_W/2, WINDOW_H/2+10);

        SDL_RenderPresent(ren);

        // cap to 60fps
        Uint32 now = SDL_GetTicks();
        Uint32 elapsed = now - last;
        if (elapsed < 16) SDL_Delay(16 - elapsed);
        last = now;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
