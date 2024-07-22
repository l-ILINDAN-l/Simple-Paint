#include "SystemSDL.h"

bool initSDL(SystemWindow* sWindow) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    sWindow->window = SDL_CreateWindow("Simple Paint", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (sWindow->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    sWindow->renderer = SDL_CreateRenderer(sWindow->window, -1, SDL_RENDERER_ACCELERATED);
    if (sWindow->renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL(SystemWindow* sWindow) {
    SDL_DestroyTexture(sWindow->canvasTexture);
    sWindow->canvasTexture = NULL;

    SDL_DestroyRenderer(sWindow->renderer);
    SDL_DestroyWindow(sWindow->window);
    sWindow->renderer = NULL;
    sWindow->window = NULL;

    IMG_Quit();
    SDL_Quit();
}

void createNewCanvas(SystemWindow* sWindow, int width, int height) {
    if (sWindow->canvasTexture != nullptr) {
        SDL_DestroyTexture(sWindow->canvasTexture);
    }
    sWindow->canvasWidth = width;
    sWindow->canvasHeight = height;
    sWindow->canvasTexture = SDL_CreateTexture(sWindow->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
        sWindow->canvasWidth, sWindow->canvasHeight);
    SDL_SetRenderTarget(sWindow->renderer, sWindow->canvasTexture);
    SDL_SetRenderDrawColor(sWindow->renderer, 255, 255, 255, 255);
    SDL_RenderClear(sWindow->renderer);
    SDL_SetRenderTarget(sWindow->renderer, NULL);
}

void drawLine(SystemWindow* sWindow, SDL_Point start, SDL_Point end) {
    SDL_SetRenderTarget(sWindow->renderer, sWindow->canvasTexture);
    SDL_RenderSetScale(sWindow->renderer, sWindow->brushSize, sWindow->brushSize);
    //SDL_SetRenderDrawColor(sWindow->renderer, sWindow->currentColor.r, sWindow->currentColor.g, sWindow->currentColor.b, sWindow->currentColor.a);
    SDL_RenderDrawLine(sWindow->renderer, (start.x-TOOLBAR_WIDTH) / sWindow->brushSize, start.y / sWindow->brushSize, end.x- TOOLBAR_WIDTH / sWindow->brushSize, end.y / sWindow->brushSize);
    SDL_SetRenderTarget(sWindow->renderer, NULL);
}


void drawRectangle(SystemWindow* sWindow, SDL_Point start, SDL_Point end) {
    SDL_RenderSetScale(sWindow->renderer, 1, 1);
    SDL_Rect rect = { (start.x - TOOLBAR_WIDTH) / sWindow->brushSize, start.y / sWindow->brushSize, (end.x - start.x) / sWindow->brushSize, (end.y - start.y) / sWindow->brushSize };
    SDL_SetRenderTarget(sWindow->renderer, sWindow->canvasTexture);
    SDL_RenderSetScale(sWindow->renderer, sWindow->brushSize, sWindow->brushSize);
    SDL_SetRenderDrawColor(sWindow->renderer, sWindow->currentColor.r, sWindow->currentColor.g, sWindow->currentColor.b, sWindow->currentColor.a);
    SDL_RenderDrawRect(sWindow->renderer, &rect);
    SDL_SetRenderTarget(sWindow->renderer, NULL);
}

void drawCircle(SystemWindow* sWindow, SDL_Point center, int radius) {
    SDL_RenderSetScale(sWindow->renderer, 1, 1);
    SDL_SetRenderTarget(sWindow->renderer, sWindow->canvasTexture);
    SDL_SetRenderDrawColor(sWindow->renderer, sWindow->currentColor.r, sWindow->currentColor.g, sWindow->currentColor.b, sWindow->currentColor.a);
    int x = radius - 1;
    int y = 0;
    int tx = 1;
    int ty = 1;
    int err = tx - (radius << 1);

    while (x >= y) {
        
        SDL_RenderDrawPoint(sWindow->renderer, center.x - TOOLBAR_WIDTH + x, center.y - y);
        SDL_RenderDrawPoint(sWindow->renderer, center.x - TOOLBAR_WIDTH + x, center.y + y);
        SDL_RenderDrawPoint(sWindow->renderer, center.x - TOOLBAR_WIDTH - x, center.y - y);
        SDL_RenderDrawPoint(sWindow->renderer, center.x - TOOLBAR_WIDTH - x, center.y + y);
        SDL_RenderDrawPoint(sWindow->renderer, center.x - TOOLBAR_WIDTH + y, center.y - x);
        SDL_RenderDrawPoint(sWindow->renderer, center.x - TOOLBAR_WIDTH + y, center.y + x);
        SDL_RenderDrawPoint(sWindow->renderer, center.x - TOOLBAR_WIDTH - y, center.y - x);
        SDL_RenderDrawPoint(sWindow->renderer, center.x - TOOLBAR_WIDTH - y, center.y + x);

        if (err <= 0) {
            y++;
            err += ty;
            ty += 2;
        }

        if (err > 0) {
            x--;
            tx += 2;
            err += (tx - (radius << 1));
        }
    }
    SDL_SetRenderTarget(sWindow->renderer, NULL);
}

void drawToolbar(SystemWindow* sWindow) {
    SDL_Rect toolbar = { 0, 0, TOOLBAR_WIDTH, WINDOW_HEIGHT };
    SDL_SetRenderDrawColor(sWindow->renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(sWindow->renderer, &toolbar);

    // Drawing buttons for tools
    SDL_Rect newButton = { 10, 10, 80, 30 };
    SDL_Rect openButton = { 10, 50, 80, 30 };
    SDL_Rect saveButton = { 10, 90, 80, 30 };
    SDL_Rect pencilButton = { 10, 130, 80, 30 };
    SDL_Rect eraserButton = { 10, 170, 80, 30 };
    SDL_Rect lineButton = { 10, 210, 80, 30 };
    SDL_Rect rectangleButton = { 10, 250, 80, 30 };
    SDL_Rect circleButton = { 10, 290, 80, 30 };
    SDL_Rect colorButton = { 10, 330, 80, 30 };
    SDL_Rect reducingSizeButton = { 10, 370, 80, 30 };
    SDL_Rect increasingSizeButton = { 10, 410, 80, 30 };

    SDL_SetRenderDrawColor(sWindow->renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(sWindow->renderer, &newButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(sWindow->renderer, &openButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 160, 160, 160, 255);
    SDL_RenderFillRect(sWindow->renderer, &saveButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(sWindow->renderer, &pencilButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 250, 250, 250, 255);
    SDL_RenderFillRect(sWindow->renderer, &eraserButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 150, 100, 100, 255);
    SDL_RenderFillRect(sWindow->renderer, &lineButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 100, 150, 100, 255);
    SDL_RenderFillRect(sWindow->renderer, &rectangleButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 100, 100, 150, 255);
    SDL_RenderFillRect(sWindow->renderer, &circleButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 150, 150, 100, 255);
    SDL_RenderFillRect(sWindow->renderer, &colorButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 150, 100, 150, 255);
    SDL_RenderFillRect(sWindow->renderer, &reducingSizeButton);

    SDL_SetRenderDrawColor(sWindow->renderer, 200, 100, 200, 255);
    SDL_RenderFillRect(sWindow->renderer, &increasingSizeButton);

    // Optionally add text labels here for buttons (requires additional setup for text rendering)
}

void chooseColor(SystemWindow* sWindow) {
    unsigned char color[3];
    const char* hexColor = tinyfd_colorChooser("Choose Color", NULL, color, color);
    if (hexColor) {
        sscanf(hexColor, "#%02hhx%02hhx%02hhx", &sWindow->currentColor.r, &sWindow->currentColor.g, &sWindow->currentColor.b);
        sWindow->currentColor.a = 255;  // Set alpha to fully opaque
    }
}

void draw(SystemWindow* sWindow) {
    SDL_RenderClear(sWindow->renderer);

    drawToolbar(sWindow);

    SDL_Rect canvasRect = { TOOLBAR_WIDTH, 0, sWindow->canvasWidth, sWindow->canvasHeight };
    SDL_RenderCopy(sWindow->renderer, sWindow->canvasTexture, NULL, &canvasRect);

    SDL_RenderPresent(sWindow->renderer);
}

void handleEvents(bool* quit, SystemWindow* sWindow) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            *quit = true;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.x > TOOLBAR_WIDTH) {
                if (sWindow->currentTool == TOOL_PENCIL || sWindow->currentTool == TOOL_ERASER) {
                    sWindow->startPoint.x = e.button.x;
                    sWindow->startPoint.y = e.button.y;
                }
                else if (sWindow->currentTool == TOOL_LINE || sWindow->currentTool == TOOL_RECTANGLE || sWindow->currentTool == TOOL_CIRCLE) {
                    sWindow->startPoint.x = e.button.x;
                    sWindow->startPoint.y = e.button.y;
                }
            }
            else {
                // Detect button presses on the toolbar
                if (e.button.y >= 10 && e.button.y <= 40) {
                    const char* input = tinyfd_inputBox("New Canvas", "Enter width and height separated by a space:", "800 600");
                    if (input) {
                        int width, height;
                        if (sscanf(input, "%d %d", &width, &height) == 2) {
                            createNewCanvas(sWindow, width, height);
                        }
                    }
                }
                else if (e.button.y >= 50 && e.button.y <= 80) {
                    const char* path = openFileDialog();
                    if (path) {
                        openPNG(path, &(sWindow->renderer), &(sWindow->canvasTexture));
                    }
                }
                else if (e.button.y >= 90 && e.button.y <= 120) {
                    savePNG("path_to_save_image.png", &(sWindow->renderer), &(sWindow->canvasTexture), sWindow->canvasWidth, sWindow->canvasHeight);
                }
                else if (e.button.y >= 130 && e.button.y <= 160) {
                    sWindow->currentTool = TOOL_PENCIL;
                }
                else if (e.button.y >= 170 && e.button.y <= 200) {
                    sWindow->currentTool = TOOL_ERASER;
                }
                else if (e.button.y >= 210 && e.button.y <= 240) {
                    sWindow->currentTool = TOOL_LINE;
                }
                else if (e.button.y >= 250 && e.button.y <= 280) {
                    sWindow->currentTool = TOOL_RECTANGLE;
                }
                else if (e.button.y >= 290 && e.button.y <= 320) {
                    sWindow->currentTool = TOOL_CIRCLE;
                }
                else if (e.button.y >= 330 && e.button.y <= 360) {
                    chooseColor(sWindow);
                }
                else if (e.button.y >= 370 && e.button.y <= 400) {
                    if (sWindow->brushSize > 1) {
                        sWindow->brushSize -= 0.01;
                    }
                }
                else if (e.button.y >= 410 && e.button.y <= 440) {
                    if (sWindow->brushSize < 1.5) {
                        sWindow->brushSize += 0.01;
                    }
                }
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            sWindow->endPoint.x = e.button.x;
            sWindow->endPoint.y = e.button.y;

            if (sWindow->currentTool == TOOL_LINE) {
                drawLine(sWindow, sWindow->startPoint, sWindow->endPoint);
            }
            else if (sWindow->currentTool == TOOL_RECTANGLE) {
                drawRectangle(sWindow, sWindow->startPoint, sWindow->endPoint);
            }
            else if (sWindow->currentTool == TOOL_CIRCLE) {
                int radius = (int)sqrt((sWindow->endPoint.x - sWindow->startPoint.x) * (sWindow->endPoint.x - sWindow->startPoint.x) + (sWindow->endPoint.y - sWindow->startPoint.y) * (sWindow->endPoint.y - sWindow->startPoint.y));
                drawCircle(sWindow, sWindow->startPoint, radius);
            }

            sWindow->startPoint.x = -1;
            sWindow->startPoint.y = -1;
            sWindow->endPoint.x = -1;
            sWindow->endPoint.y = -1;
        }
        else if (e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON_LMASK)) {
            if (sWindow->currentTool == TOOL_PENCIL || sWindow->currentTool == TOOL_ERASER) {
                sWindow->endPoint.x = e.motion.x;
                sWindow->endPoint.y = e.motion.y;

                if (sWindow->currentTool == TOOL_PENCIL) {
                    SDL_SetRenderDrawColor(sWindow->renderer, sWindow->currentColor.r, sWindow->currentColor.g, sWindow->currentColor.b, sWindow->currentColor.a);
                }
                else if (sWindow->currentTool == TOOL_ERASER) {
                    SDL_SetRenderDrawColor(sWindow->renderer, 255, 255, 255, 255);
                }

                drawLine(sWindow, sWindow->startPoint, sWindow->endPoint);

                sWindow->startPoint.x = sWindow->endPoint.x;
                sWindow->startPoint.y = sWindow->endPoint.y;
            }
        }
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_o) {
                const char* path = openFileDialog();
                if (path) {
                    openPNG(path, &(sWindow->renderer), &(sWindow->canvasTexture));
                }
            }
            else if (e.key.keysym.sym == SDLK_s) {
                savePNG("path_to_save_image.png", &(sWindow->renderer), &(sWindow->canvasTexture), sWindow->canvasWidth, sWindow->canvasHeight);
            }
        }
    }
}