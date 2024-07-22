#pragma once
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
#include "Tool.h"
#include "tinyfiledialogs.h"
#include "SystemPNG.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TOOLBAR_WIDTH 100

typedef struct SystemWindow {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* canvasTexture = nullptr;
	int canvasWidth = WINDOW_WIDTH - TOOLBAR_WIDTH;
	int canvasHeight = WINDOW_HEIGHT;
	Tool currentTool = TOOL_NONE;
	float brushSize = 1;
	SDL_Color currentColor = { 0, 0, 0, 255 };
	SDL_Point startPoint = { -1, -1 };
	SDL_Point endPoint = { -1, -1 };
} SystemWindow;

bool initSDL(SystemWindow* sWindow);
void closeSDL(SystemWindow* sWindow);
void createNewCanvas(SystemWindow* sWindow, int width, int height);
void handleEvents(bool* quit, SystemWindow* sWindow);
void draw(SystemWindow* sWindow);