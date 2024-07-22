#pragma once
#include <png.h>
#include "tinyfiledialogs.h"
#include <SDL.h>
#include <SDL_image.h>

#pragma warning(disable : 4996)
const char* openFileDialog();
void openPNG(const char* filePath, SDL_Renderer** renderer, SDL_Texture** canvasTexture);
void savePNG(const char* filePath, SDL_Renderer** renderer, SDL_Texture** canvasTexture, int canvasWidth, int canvasHeight);