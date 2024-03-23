#include <stdlib.h>

#include <SDL3/SDL.h>

#include <gooeLog/log.h>

#include "types.h"
#include "window.h"
#include "const.h"

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)

void gooe_start(GooE* gooe) {
    if (SDL_Init(SDL_INIT_FLAGS) < 0) {
        LOG_ERR("GooE start fails => SDL could not initialise: %s", SDL_GetError());
        exit(1);
    }
    LOG_INFO("Gooe start success.");

    gooe->window = SDL_CreateWindow(GOOE_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!gooe->window) {
        LOG_ERR("Gooe window creation fails => SDL failed to create window: %s", SDL_GetError());
        gooe_stop(gooe);
        exit(1);
    }
    LOG_INFO("Gooe window creation success.");
}

void gooe_stop(GooE* gooe) {
    if (gooe->window) {
        SDL_DestroyWindow(gooe->window);
        gooe->window = NULL;
        LOG_INFO("Gooe window destroyed.");
    }

    SDL_Quit();
    LOG_INFO("Gooe stopped.");
}