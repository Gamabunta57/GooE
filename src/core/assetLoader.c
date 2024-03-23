#include <SDL_render.h>
#include <SDL_surface.h>
#include <stdlib.h>

#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer//SDL_mixer.h>

#include <gooeLog/log.h>

#include "assetLoader.h"
#include "core/gooe.h"
#include "core/types.h"

static void* texture = NULL;
static void* music = NULL;
static int imgInit = -1;

void gooe_assetInit() {
    imgInit = IMG_Init(IMG_INIT_PNG);
    if (imgInit < 0) {
        LOG_ERR("GooE assets loader => SDL image could not initialise: %s", SDL_GetError());
        exit(1);
    }

    LOG_INFO("GooE images asset loader initialisation success.");
}

void gooe_assetDestroy() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        LOG_INFO("GooE textures destroyed.");
    }

    if (imgInit >= 0) {
        IMG_Quit();
        LOG_INFO("GooE images asset loader destroyed.");
    }

    if (music) {
        Mix_FreeMusic(music);
        music = NULL;
        LOG_INFO("GooE music destroyed.");
    }
}

void* gooe_assetLoadImage(void* renderer, const char* assetPath) {
    SDL_Surface* image = IMG_Load(assetPath);
    if (!image) {
        LOG_ERR("GooE image asset loader fails => SDL image could load surface: %s", SDL_GetError());
        gooe_destroy();
        exit(1);
    }
    LOG_INFO("GooE image successfully loaded as surface");

    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (!texture) {
        LOG_ERR("GooE image asset loader fails => SDL could not create the texture out from the surface: %s", SDL_GetError());
        SDL_DestroySurface(image);
        gooe_destroy();
        exit(1);
    }
    LOG_INFO("GooE image asset loader texture created successfully");
    SDL_DestroySurface(image);
    return texture;
}

void* gooe_assetLoadMusic(const char* assetPath) {
    music = Mix_LoadMUS_RW(SDL_RWFromFile(assetPath, "rb"), SDL_TRUE);

    if (!music) {
        LOG_ERR("SDL mixer could not load the music: %s", SDL_GetError());
        gooe_destroy();
        exit(1);
    }
    LOG_INFO("GooE music loaded successfully.");
    return music;
}
