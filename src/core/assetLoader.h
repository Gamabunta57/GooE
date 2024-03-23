#pragma once

#include "types.h"

void gooe_assetInit();
void gooe_assetDestroy();

void* gooe_assetLoadImage(void* renderer, const char* assetPath);
void* gooe_assetLoadMusic(const char* assetPath);
