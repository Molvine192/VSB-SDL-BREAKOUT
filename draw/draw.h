#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Draws a rectangle with a specified color at a given position on the screen
 * @param rect Specifies the position and dimensions of the rectangle
 * @param color Sets the color of the rectangle
 */
void MB_drawRect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color);

/**
 * @brief Draws an image with a specified color at a given position on the screen
 * @param texture The texture of the image
 * @param rect Specifies the position and dimensions of the image
 * @param color Sets the color of the image
 */
void MB_drawImage(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect rect, SDL_Color color);

/**
 * @brief Draws text with a specified color at a given position on the screen
 * @param text The text to be drawn
 * @param font The font of the text
 * @param x The position of the text along the width
 * @param y The position of the text along the height
 * @param alignX Alignment of the text along the width
 * @param alignY Alignment of the text along the height
 * @param color The color of the text
 */
void MB_drawText(SDL_Renderer* renderer, char* text, TTF_Font* font, int x, int y, char* alignX, char* alignY, SDL_Color color);

/**
 * @brief Draws text with a specified color at a given position on the screen with an outline
 * @param text The text to be drawn
 * @param font The font of the text
 * @param x The position of the text along the width
 * @param y The position of the text along the height
 * @param alignX Alignment of the text along the width
 * @param alignY Alignment of the text along the height
 * @param color The color of the text
 * @param border The width of the outline
 */
void MB_drawBordererText(SDL_Renderer* renderer, char* text, TTF_Font* font, int x, int y, char* alignX, char* alignY, SDL_Color color, int border);

#endif
