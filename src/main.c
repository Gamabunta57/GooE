#include <process.h>
#include <stdio.h>
#include <SDL3/SDL.h>

#define GOOE_LOG_IMPLEMENTATION
#include <gooeLog/log.h>
#include "const.h"

void init();
void quit();
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

int main(int argc, char** argv) {
    LOG_INFO("For your %s", "INFO");
    LOG_DEBUG("For your %s", "DEBUG");
    LOG_WARN("For your %s", "WARNING");
    LOG_ERR("For your %s", "ERROR");

    init();
    SDL_Event event;
    unsigned char shouldQuit = 0;
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
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    quit();
}

void init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERR("SDL could not initialise: %s", SDL_GetError());
        exit(1);
    }
    LOG_INFO("SDL initialisation success.");

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
}

void quit() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}
