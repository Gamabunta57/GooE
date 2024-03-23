#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <gooeLog/log.h>
#include "meta.h"
#include "const.h"
#include "core/gooe.h"

void init();
void quit();
static SDL_Surface* image = NULL;
static SDL_Texture* texture = NULL;
static Mix_Music* music = NULL;
static double loop_start = 0.0;
static double loop_end = 0.0;
static double loop_length = 0.0;

GooE* gooe;

int main(int argc, char** argv) {
    LOG_INFO("Booting GooE v%s (%s)", GOOE_VERSION, GOOE_COMPILE_MODE);

    init();
    SDL_Event event;
    unsigned char shouldQuit = 0;
    SDL_FRect srcRect = {
        .x = 0, .y = 0,
        .w = 32, .h = 32
    };
    SDL_FRect dstRect = {
        .x = (SCREEN_WIDTH - 32) / 2.0f, .y = (SCREEN_HEIGHT - 32) / 2.0f,
        .w = 32, .h = 32
    };
    while(!shouldQuit) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_EVENT_QUIT: {
                    shouldQuit = 1;
                }
            }
        }
        SDL_SetRenderDrawColor(gooe->renderer, 80, 10, 160, 255);
        SDL_RenderClear(gooe->renderer);
        SDL_RenderTexture(gooe->renderer, texture, &srcRect, &dstRect);
        SDL_RenderPresent(gooe->renderer);
        dstRect.x += 2;
        SDL_Delay(16);
    }
    quit();
}

void init() {
    gooe = gooe_init();

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        LOG_ERR("SDL image could not initialise: %s", SDL_GetError());
        exit(1);
    }

    SDL_AudioSpec spec = {
        .freq = MIX_DEFAULT_FREQUENCY,
        .format = MIX_DEFAULT_FORMAT,
        .channels = MIX_DEFAULT_CHANNELS,
    };

    if (Mix_OpenAudio(0, &spec) < 0) {
        LOG_ERR("SDL mixer could not initialise: %s", SDL_GetError());
        exit(1);
    } 
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    music = Mix_LoadMUS_RW(SDL_RWFromFile("assets/music/test.ogg", "rb"), SDL_TRUE);

    if (!music) {
        LOG_ERR("SDL mixer could not load the music: %s", SDL_GetError());
        exit(1);
    }
    LOG_INFO("SDL music loaded successfully.");

    loop_start = Mix_GetMusicLoopStartTime(music);
    loop_end = Mix_GetMusicLoopEndTime(music);
    loop_length = Mix_GetMusicLoopLengthTime(music);

    #ifndef GOOE_LOG_NONE
    double duration = Mix_MusicDuration(music);
    LOG_INFO("Loaded music duration is: %fsec", duration);
    #endif

    if (loop_start > 0.0 && loop_end > 0.0 && loop_length > 0.0) {
        LOG_INFO("Loop points: start %g sec, end %g sec, length %g sec", loop_start, loop_end, loop_length);
    } else {
        LOG_INFO("The loaded music is not looping.");
    }

    Mix_FadeInMusic(music, -1, 0);

    image = IMG_Load("assets/img/test.png");
    if (!image) {
        LOG_ERR("SDL image could load surface: %s", SDL_GetError());
        exit(1);
    }
    LOG_INFO("SDL image successfully loaded the surface");

    texture = SDL_CreateTextureFromSurface(gooe->renderer, image);
    if (!texture) {
        LOG_ERR("SDL could not create the texture out from the surface: %s", SDL_GetError());
    }
    LOG_INFO("SDL texture created successfully");
}

void quit() {
    Mix_FreeMusic(music);
    music = NULL;
    SDL_DestroyTexture(texture);
    texture = NULL;
    SDL_DestroySurface(image);
    image = NULL;
    IMG_Quit();

    gooe_destroy();
}
