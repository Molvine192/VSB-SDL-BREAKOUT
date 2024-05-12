#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../draw/draw.h"

void MB_drawRect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void MB_drawImage(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect rect, SDL_Color color) {
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    SDL_SetTextureColorMod(texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(texture, 255);
}

void MB_drawText(SDL_Renderer* renderer, char* text, TTF_Font* font, int x, int y, char* alignX, char* alignY, SDL_Color color) {
    int w = 0, h = 0;
    TTF_SizeUTF8(font, text, &w, &h);
    SDL_Rect rect = {0,0,w,h};

    if (strcmp(alignX, "right") == 0)
        rect.x = x - w;
    else if (strcmp(alignX, "left") == 0)
        rect.x = x;
    else if (strcmp(alignX, "center") == 0)
        rect.x = x - w/2;
    else
        return;

    if (strcmp(alignY, "top") == 0)
        rect.y = y;
    else if (strcmp(alignY, "bottom") == 0)
        rect.y = y-h;
    else if (strcmp(alignY, "center") == 0)
        rect.y = y - h/2;
    else
        return;

    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    SDL_DestroyTexture(texture);
}

void MB_drawBordererText(SDL_Renderer* renderer, char* text, TTF_Font* font, int x, int y, char* alignX, char* alignY, SDL_Color color, int border) {

    int w = 0, h = 0;
    TTF_SizeUTF8(font, text, &w, &h);
    SDL_Rect rect = {0,0,w,h};

    if (strcmp(alignX, "right") == 0)
        rect.x = x - w;
    else if (strcmp(alignX, "left") == 0)
        rect.x = x;
    else if (strcmp(alignX, "center") == 0)
        rect.x = x - w/2;
    else
        return;

    if (strcmp(alignY, "top") == 0)
        rect.y = y;
    else if (strcmp(alignY, "bottom") == 0)
        rect.y = y-h;
    else if (strcmp(alignY, "center") == 0)
        rect.y = y - h/2;
    else
        return;

    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text, (SDL_Color){0,0,0,255});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopyEx(renderer, texture, NULL, &((SDL_Rect){rect.x,rect.y+border,rect.w,rect.h}), 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, texture, NULL, &((SDL_Rect){rect.x+border,rect.y+border,rect.w,rect.h}), 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, texture, NULL, &((SDL_Rect){rect.x+border,rect.y,rect.w,rect.h}), 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, texture, NULL, &((SDL_Rect){rect.x+border,rect.y-border,rect.w,rect.h}), 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, texture, NULL, &((SDL_Rect){rect.x,rect.y-border,rect.w,rect.h}), 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, texture, NULL, &((SDL_Rect){rect.x-border,rect.y-border,rect.w,rect.h}), 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, texture, NULL, &((SDL_Rect){rect.x-border,rect.y,rect.w,rect.h}), 0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, texture, NULL, &((SDL_Rect){rect.x-border,rect.y+border,rect.w,rect.h}), 0, NULL, SDL_FLIP_NONE);

    SDL_DestroyTexture(texture);

    surface = TTF_RenderUTF8_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);

    SDL_DestroyTexture(texture);
}