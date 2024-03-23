#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <gooeLog/log.h>

#include "audio.h"
#include "core/gooe.h"

void _readMusicInfo(void* music);

void gooe_audioInit() {
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

    Mix_VolumeMusic(MIX_MAX_VOLUME);

    LOG_INFO("GooE audio initialisation success.");
}

void gooe_audioDestroy() {
    Mix_Quit();
    LOG_INFO("GooE audio destroyed.");
}

void gooe_audioPlayLoop(void* musicAsset) {
    #ifndef NDEBUG
    _readMusicInfo(musicAsset);
    #endif

    Mix_FadeInMusic(musicAsset, -1, 0);
}

void _readMusicInfo(void* asset) {
    double loop_start = Mix_GetMusicLoopStartTime(asset);
    double loop_end = Mix_GetMusicLoopEndTime(asset);
    double loop_length = Mix_GetMusicLoopLengthTime(asset);

    #ifdef DEBUG
    double duration = Mix_MusicDuration(asset);
    LOG_INFO("Loaded music duration is: %fsec", duration);
    #endif

    if (loop_start > 0.0 && loop_end > 0.0 && loop_length > 0.0) {
        LOG_INFO("Loop points: start %g sec, end %g sec, length %g sec", loop_start, loop_end, loop_length);
    } else {
        LOG_INFO("The loaded music is not looping.");
    }
}
