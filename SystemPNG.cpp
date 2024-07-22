#include "SystemPNG.h"


const char* openFileDialog() {
    const char* filterPatterns[] = { "*.png", "*.PNG" };
    const char* path = tinyfd_openFileDialog(
        "Open PNG File",
        "",
        2,
        filterPatterns,
        NULL,
        0
    );

    return path;
}

void openPNG(const char* filePath, SDL_Renderer** renderer, SDL_Texture** canvasTexture) {
    SDL_Surface* loadedSurface = IMG_Load(filePath);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", filePath, IMG_GetError());
        return;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(*renderer, loadedSurface);
    if (newTexture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", filePath, SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        return;
    }

    SDL_FreeSurface(loadedSurface);
    if (canvasTexture != NULL) {
        SDL_DestroyTexture(*canvasTexture);
    }
    *canvasTexture = newTexture;
}

void savePNG(const char* filePath, SDL_Renderer** renderer, SDL_Texture** canvasTexture, int canvasWidth, int canvasHeight) {
    int width = canvasWidth;
    int height = canvasHeight;
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_SetRenderTarget(*renderer, *canvasTexture);
    SDL_RenderReadPixels(*renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    SDL_SetRenderTarget(*renderer, NULL);

    FILE* fp = fopen(filePath, "wb");
    if (!fp) {
        printf("Could not open file for writing\n");
        SDL_FreeSurface(surface);
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        printf("Could not create png write struct\n");
        fclose(fp);
        SDL_FreeSurface(surface);
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        printf("Could not create png info struct\n");
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        SDL_FreeSurface(surface);
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        printf("Error during png creation\n");
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        SDL_FreeSurface(surface);
        return;
    }

    png_init_io(png, fp);
    png_set_IHDR(
        png, info, width, height,
        8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    for (int y = 0; y < height; y++) {
        png_bytep row = (png_bytep)((Uint8*)surface->pixels + y * surface->pitch);
        png_write_row(png, row);
    }

    png_write_end(png, NULL);
    fclose(fp);
    png_destroy_write_struct(&png, &info);
    SDL_FreeSurface(surface);
}

