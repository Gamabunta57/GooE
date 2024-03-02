#include <process.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define GOOE_LOG_IMPLEMENTATION
#include <gooeLog/log.h>
#include "const.h"

void init();
void quit();
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Surface* image = NULL;
static SDL_Texture* texture = NULL;

int main(int argc, char** argv) {
    LOG_INFO("For your %s", "INFO");
    LOG_DEBUG("For your %s", "DEBUG");
    LOG_WARN("For your %s", "WARNING");
    LOG_ERR("For your %s", "ERROR");

    init();
    SDL_Event event;
    unsigned char shouldQuit = 0;
    SDL_FRect srcRect = {
        .x = 0, .y = 0,
        .w = 32, .h = 32
    };
    SDL_FRect dstRect = {
        .x = (SCREEN_WIDTH - 32) / 2, .y = (SCREEN_HEIGHT - 32) / 2,
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
        SDL_SetRenderDrawColor(renderer, 80, 10, 160, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, &srcRect, &dstRect);
        SDL_RenderPresent(renderer);
        dstRect.x += 2;
        SDL_Delay(16);
    }
    quit();
}

void init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERR("SDL could not initialise: %s", SDL_GetError());
        exit(1);
    }
    LOG_INFO("SDL initialisation success.");

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        LOG_ERR("SDL image could not initialise: %s", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("GooE", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        LOG_ERR("SDL failed to create window: %s", SDL_GetError());
        exit(1);
    }
    LOG_INFO("SDL window creation success.");

    renderer = SDL_CreateRenderer(window, NULL, 0);
    if (!renderer) {
        LOG_ERR("SDL failed to create the renderer: %s", SDL_GetError());
        exit(1);
    }

    image = IMG_Load("assets/img/test.png");
    if (!image) {
        LOG_ERR("SDL image could load surface: %s", SDL_GetError());
        exit(1);
    }
    LOG_INFO("SDL image successfully loaded the surface");

    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (!texture) {
        LOG_ERR("SDL could not create the texture out from the surface: %s", SDL_GetError());
    }
    LOG_INFO("SDL texture created successfully");
}

void quit() {
    SDL_DestroyTexture(texture);
    texture = NULL;
    SDL_DestroySurface(image);
    image = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    IMG_Quit();
    SDL_Quit();
}
