#include <stdlib.h>
#include <SDL3/SDL.h>
#include <gooeLog/log.h>

#include "core/assetLoader.h"
#include "core/audio.h"
#include "core/renderer.h"
#include "types.h"
#include "window.h"
#include "gooe.h"

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)

static GooE gooe = {};

void _init();
void _destroy();

GooE* gooe_init() {
    _init();
    gooe_windowInit(&gooe);
    gooe_rendererInit(&gooe);
    gooe_audioInit();
    gooe_assetInit();
    return &gooe;
}

void gooe_destroy() {
    gooe_assetDestroy();
    gooe_audioDestroy();
    gooe_rendererDestroy(&gooe);
    gooe_windowDestroy(&gooe);
    _destroy();
}

void _init() {
    if (SDL_Init(SDL_INIT_FLAGS) < 0) {
        LOG_ERR("GooE init fails => SDL could not initialise: %s", SDL_GetError());
        exit(1);
    }
    LOG_INFO("GooE init success.");
}

void _destroy() {
    SDL_Quit();
    LOG_INFO("GooE destroyed.");
}
