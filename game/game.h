#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../dynamic_array/dynamic_array.h"

/**
 * @brief Starts the game
 */
void MB_start();

/**
 * @brief Loads the level from a text file
 * @param field Pointer to a dynamic array where information about bricks needs to be loaded
 * @param id Level identifier
 */
void loadLevel(dynarray *field, char* id);

/**
 * @brief Callback function for the score timer
 */
Uint32 callbackScoreTimer(Uint32 interval, void* par);

/**
 * @brief Callback function for the modifier timer
 */
Uint32 callbackModfTimer(Uint32 interval, void* par);

/**
 * @brief Callback function for the background timer
 */
Uint32 callbackBackgroundTimer(Uint32 interval, void* par);

/**
 * @brief Returns the color for a given brick color index
 * @param index Index of the brick color
 * @return SDL_Color The color of the brick
 */
SDL_Color getBrickColorByIndex(char index);

/**
 * @brief Callback function for the game over timer
 * @param score Player's score
 * @param scene Scene identifier
 */
void onGameOver(int score, enum GameScene scene);

/**
 * @brief Update the top 5 players
 */
void refreshTop5Players();

/**
 * @brief Reset game data to default
 */
void refreshGameToDefault();

/**
 * @brief Smoothly fade the music volume
 * @param flag Value for fading. true - increase volume, false - decrease volume
 */
void fadeMusic(bool flag);

#endif
