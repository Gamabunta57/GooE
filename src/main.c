#include <process.h>
#include <stdio.h>
#define GOOE_LOG_IMPLEMENTATION
#include <gooeLog/log.h>
#include <SDL3/SDL.h>

void init();
void quit();

int main(int argc, char** argv) {
    LOG_INFO("For your %s", "INFO");
    LOG_DEBUG("For your %s", "DEBUG");
    LOG_WARN("For your %s", "WARNING");
    LOG_ERR("For your %s", "ERROR");

    init();
    quit();
}

void init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERR("SDL could not initialise");
        exit(1);
    }
    LOG_INFO("SDL initialisation success.");
}

void quit() {
    SDL_Quit();
}
