#include <stdlib.h>
#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <gooeLog/log.h>

#include "core/gooe.h"
#include "core/audio.h"
#include "core/assetLoader.h"
#include "meta.h"
#include "const.h"

void init();
void quit();

GooE* gooe;
void* texture;
void* music;

int main() {
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

    music = gooe_assetLoadMusic("assets/music/test.ogg");
    texture = gooe_assetLoadImage(gooe->renderer, "assets/img/test.png");

    gooe_audioPlayLoop(music);
}

void quit() {
    gooe_destroy();
}
