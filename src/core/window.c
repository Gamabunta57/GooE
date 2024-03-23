#include <stdlib.h>

#include <SDL3/SDL.h>

#include <gooeLog/log.h>

#include "types.h"
#include "window.h"
#include "const.h"
#include "gooe.h"

void gooe_windowInit(GooE* gooe) {
    gooe->window = SDL_CreateWindow(GOOE_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!gooe->window) {
        LOG_ERR("GooE window creation fails => SDL failed to create window: %s", SDL_GetError());
        gooe_destroy();
        exit(1);
    }
    LOG_INFO("GooE window creation success.");
}

void gooe_windowDestroy(GooE* gooe) {
    if (!gooe->window) return;

    SDL_DestroyWindow(gooe->window);
    gooe->window = NULL;
    LOG_INFO("GooE window destroyed.");
}
