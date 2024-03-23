#include <stdlib.h>

#include <SDL3/SDL.h>

#include <gooeLog/log.h>

#include "core/gooe.h"
#include "renderer.h"

void gooe_rendererInit(GooE* gooe) {
    gooe->renderer = SDL_CreateRenderer(gooe->window, NULL, 0);
    if (!gooe->renderer) {
        LOG_ERR("GooE renderer init fails => SDL failed to create the renderer: %s", SDL_GetError());
        gooe_destroy();
        exit(1);
    }
    LOG_INFO("GooE renderer initialisation success.");
}

void gooe_rendererDestroy(GooE* gooe) {
    if (!gooe->renderer) return;

    SDL_DestroyRenderer(gooe->renderer);
    gooe->renderer = NULL;
    LOG_INFO("GooE rendered destroyed.");
}
