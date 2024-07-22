#include "Main.h"

int main(int argc, char* args[]) {
    SystemWindow sWindow;
    if (!initSDL(&sWindow)) {
        printf_s("Failed to initialize!\n");
    }

    else {
        bool quit = false;

        createNewCanvas(&sWindow, sWindow.canvasWidth, sWindow.canvasHeight);

        while (!quit) {
            handleEvents(&quit, &sWindow);
            draw(&sWindow);
        }
    }
    closeSDL(&sWindow);
	return 0;
}





//#pragma warning(disable : 4996)
//
//#include <SDL.h>
//#include <stdbool.h>
//#include <SDL_image.h>
//#include <SDL_ttf.h>
//
//// Константы для размеров окна и панели инструментов
//#define WINDOW_WIDTH 800
//#define WINDOW_HEIGHT 600
//#define TOOLBAR_HEIGHT 50
//#define COLOR_PICKER_WIDTH 50
//#define TOOL_PICKER_WIDTH 50
//#define THICKNESS_PICKER_WIDTH 50
//
//// Перечисление для инструментов
//typedef enum {
//    TOOL_PENCIL,
//    TOOL_ERASER
//} Tool;
//
//// Структура для хранения состояния приложения
//typedef struct {
//    SDL_Color currentColor;
//    int toolSize;
//    Tool currentTool;
//} AppState;
//
//// Функция инициализации SDL
//bool initSDL(SDL_Window** window, SDL_Renderer** renderer) {
//    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
//        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
//        return false;
//    }
//    *window = SDL_CreateWindow("Simple Paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
//    if (!*window) {
//        SDL_Log("Unable to create window: %s", SDL_GetError());
//        SDL_Quit();
//        return false;
//    }
//    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//    if (!*renderer) {
//        SDL_Log("Unable to create renderer: %s", SDL_GetError());
//        SDL_DestroyWindow(*window);
//        SDL_Quit();
//        return false;
//    }
//    return true;
//}
//
//// Функция отрисовки интерфейса (палитра цветов и инструменты)
//void drawUI(SDL_Renderer* renderer, AppState* state) {
//    // Рисуем панель инструментов
//    SDL_Rect toolbar = { 0, 0, WINDOW_WIDTH, TOOLBAR_HEIGHT };
//    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
//    SDL_RenderFillRect(renderer, &toolbar);
//
//    // Рисуем палитру цветов
//    SDL_Color colors[] = {
//        {255, 0, 0, 255},   // Red
//        {0, 255, 0, 255},   // Green
//        {0, 0, 255, 255},   // Blue
//        {255, 255, 0, 255}, // Yellow
//        {0, 255, 255, 255}, // Cyan
//        {255, 0, 255, 255}, // Magenta
//        {0, 0, 0, 255},     // Black
//        {255, 255, 255, 255}// White
//    };
//    for (int i = 0; i < 8; ++i) {
//        SDL_Rect colorRect = { i * COLOR_PICKER_WIDTH, 0, COLOR_PICKER_WIDTH, TOOLBAR_HEIGHT };
//        SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
//        SDL_RenderFillRect(renderer, &colorRect);
//    }
//
//    // Рисуем индикатор текущего инструмента
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//    SDL_Rect pencilRect = { 8 * COLOR_PICKER_WIDTH, 0, TOOL_PICKER_WIDTH, TOOLBAR_HEIGHT };
//    SDL_Rect eraserRect = { 8 * COLOR_PICKER_WIDTH + TOOL_PICKER_WIDTH, 0, TOOL_PICKER_WIDTH, TOOLBAR_HEIGHT };
//    SDL_RenderDrawRect(renderer, &pencilRect);
//    SDL_RenderDrawRect(renderer, &eraserRect);
//    if (state->currentTool == TOOL_PENCIL) {
//        SDL_RenderDrawLine(renderer, 8 * COLOR_PICKER_WIDTH, 0, 8 * COLOR_PICKER_WIDTH + TOOL_PICKER_WIDTH, TOOLBAR_HEIGHT);
//    }
//    else if (state->currentTool == TOOL_ERASER) {
//        SDL_RenderDrawLine(renderer, 8 * COLOR_PICKER_WIDTH + TOOL_PICKER_WIDTH, 0, 8 * COLOR_PICKER_WIDTH + 2 * TOOL_PICKER_WIDTH, TOOLBAR_HEIGHT);
//    }
//
//    // Рисуем кнопки изменения толщины
//    SDL_Rect thicknessDecreaseRect = { 8 * COLOR_PICKER_WIDTH + 2 * TOOL_PICKER_WIDTH, 0, THICKNESS_PICKER_WIDTH, TOOLBAR_HEIGHT };
//    SDL_Rect thicknessIncreaseRect = { 8 * COLOR_PICKER_WIDTH + 2 * TOOL_PICKER_WIDTH + THICKNESS_PICKER_WIDTH, 0, THICKNESS_PICKER_WIDTH, TOOLBAR_HEIGHT };
//    SDL_RenderDrawRect(renderer, &thicknessDecreaseRect);
//    SDL_RenderDrawRect(renderer, &thicknessIncreaseRect);
//
//    // Текущая толщина инструмента
//    char thicknessText[16];
//    sprintf(thicknessText, "%d", state->toolSize);
//    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, THICKNESS_PICKER_WIDTH, TOOLBAR_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
//    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 200, 200, 200, 255));
//    SDL_Color textColor = { 0, 0, 0, 255 };
//    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
//    SDL_Surface* textSurface = TTF_RenderText_Solid(font, thicknessText, textColor);
//    SDL_BlitSurface(textSurface, NULL, surface, NULL);
//    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//    SDL_Rect textRect = { 8 * COLOR_PICKER_WIDTH + 2 * TOOL_PICKER_WIDTH + 2 * THICKNESS_PICKER_WIDTH, 0, THICKNESS_PICKER_WIDTH, TOOLBAR_HEIGHT };
//    SDL_RenderCopy(renderer, texture, NULL, &textRect);
//
//    SDL_FreeSurface(surface);
//    SDL_FreeSurface(textSurface);
//    SDL_DestroyTexture(texture);
//    TTF_CloseFont(font);
//}
//
//// Обработка ввода от пользователя
//void handleInput(SDL_Event* event, AppState* state, SDL_Renderer* renderer) {
//    if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEMOTION) {
//        if (event->motion.y < TOOLBAR_HEIGHT) {
//            if (event->motion.x < 8 * COLOR_PICKER_WIDTH) {
//                int colorIndex = event->motion.x / COLOR_PICKER_WIDTH;
//                SDL_Color colors[] = {
//                    {255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255},
//                    {255, 255, 0, 255}, {0, 255, 255, 255}, {255, 0, 255, 255},
//                    {0, 0, 0, 255}, {255, 255, 255, 255}
//                };
//                state->currentColor = colors[colorIndex];
//            }
//            else if (event->motion.x < 8 * COLOR_PICKER_WIDTH + TOOL_PICKER_WIDTH) {
//                state->currentTool = TOOL_PENCIL;
//            }
//            else if (event->motion.x < 8 * COLOR_PICKER_WIDTH + 2 * TOOL_PICKER_WIDTH) {
//                state->currentTool = TOOL_ERASER;
//            }
//            else if (event->motion.x < 8 * COLOR_PICKER_WIDTH + 2 * TOOL_PICKER_WIDTH + THICKNESS_PICKER_WIDTH) {
//                if (state->toolSize > 1) {
//                    state->toolSize -= 1;
//                }
//            }
//            else if (event->motion.x < 8 * COLOR_PICKER_WIDTH + 2 * TOOL_PICKER_WIDTH + 2 * THICKNESS_PICKER_WIDTH) {
//                state->toolSize += 1;
//            }
//        }
//        else {
//            if (event->type == SDL_MOUSEBUTTONDOWN || (event->type == SDL_MOUSEMOTION && (event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)))) {
//                int x = event->motion.x;
//                int y = event->motion.y;
//
//                if (state->currentTool == TOOL_PENCIL) {
//                    SDL_SetRenderDrawColor(renderer, state->currentColor.r, state->currentColor.g, state->currentColor.b, state->currentColor.a);
//                }
//                else if (state->currentTool == TOOL_ERASER) {
//                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Белый цвет для ластика
//                }
//
//                SDL_Rect drawRect = { x - state->toolSize / 2, y - state->toolSize / 2, state->toolSize, state->toolSize };
//                SDL_RenderFillRect(renderer, &drawRect);
//            }
//        }
//    }
//}
//
//int main(int argc, char* argv[]) {
//    SDL_Window* window = NULL;
//    SDL_Renderer* renderer = NULL;
//    if (!initSDL(&window, &renderer)) {
//        return -1;
//    }
//
//    if (TTF_Init() == -1) {
//        SDL_Log("Unable to initialize TTF: %s", TTF_GetError());
//        SDL_DestroyRenderer(renderer);
//        SDL_DestroyWindow(window);
//        SDL_Quit();
//        return -1;
//    }
//
//    AppState state = { {0, 0, 0, 255}, 5, TOOL_PENCIL };
//    bool quit = false;
//    SDL_Event event;
//
//    while (!quit) {
//        while (SDL_PollEvent(&event)) {
//            if (event.type == SDL_QUIT) {
//                quit = true;
//            }
//            else {
//                handleInput(&event, &state, renderer);
//            }
//        }
//
//        drawUI(renderer, &state);
//        SDL_RenderPresent(renderer);
//    }
//
//    TTF_Quit();
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//    return 0;
//}