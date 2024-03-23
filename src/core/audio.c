#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <gooeLog/log.h>

#include "audio.h"
#include "core/gooe.h"

void gooe_audioInit(GooE* gooe) {
    SDL_AudioSpec spec = {
        .freq = MIX_DEFAULT_FREQUENCY,
        .format = MIX_DEFAULT_FORMAT,
        .channels = MIX_DEFAULT_CHANNELS,
    };

    if (Mix_OpenAudio(0, &spec) < 0) {
        LOG_ERR("GooE audio init fails => SDL mixer could not initialise: %s", SDL_GetError());
        gooe_destroy();
        exit(1);
    }

    LOG_INFO("GooE audio initialisation success.");
}

void gooe_audioDestroy(GooE* gooe) {
    Mix_Quit();
    LOG_INFO("GooE audio destroyed.");
}
