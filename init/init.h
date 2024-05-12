#ifndef INIT_H
#define INIT_H

#include <SDL2/SDL.h>
#include "../dynamic_array/dynamic_array.h"

/**
 * @brief Enumerator for game scene states
 * @param SCENE_MAIN_MENU Main menu
 * @param SCENE_SELECT_LEVEL Level selection
 * @param SCENE_GAME Game
 * @param SCENE_GAME_WIN Victory screen
 * @param SCENE_GAME_OVER Game over screen
 */
enum GameScene {
    SCENE_MAIN_MENU,
    SCENE_SELECT_LEVEL,
    SCENE_GAME,
    SCENE_GAME_WIN,
    SCENE_GAME_OVER
};

/**
 * @brief Enumerator for ball states
 * @param BALL_STOPPED Ball is not moving
 * @param BALL_MOVE_UP Ball is moving upwards
 * @param BALL_MOVE_DOWN Ball is moving downwards
 */
enum BallStatus {
    BALL_STOPPED,
    BALL_MOVE_UP,
    BALL_MOVE_DOWN
};

/**
 * @brief Enumerator for platform states
 * @param PLATFORM_STOPPED Platform is not moving
 * @param PLATFORM_MOVE_RIGHT Platform is moving right
 * @param PLATFORM_MOVE_LEFT Platform is moving left
 * @param PLATFORM_BOTH Platform is not moving (attempt to move in both directions simultaneously)
 */
enum PlatformStatus {
    PLATFORM_STOPPED,
    PLATFORM_MOVE_RIGHT,
    PLATFORM_MOVE_LEFT,
    PLATFORM_BOTH
};

/**
 * @brief Structure for "Brick"
 * @param posX Position on the x-axis
 * @param posY Position on the y-axis
 * @param color Color
 */
typedef struct {
    int posX;
    int posY;
    char color;
} Brick;

/**
 * @brief Structure for "Rectangle"
 * @param x Position on the x-axis
 * @param y Position on the y-axis
 * @param w Width
 * @param h Height
 */
typedef struct {
    float x;
    float y;
    float w;
    float h;
} MB_Rect;

/**
 * @brief Structure for "Top Player"
 * @param score Score
 * @param name Nickname
 */
typedef struct {
    int score;
    char name[16];
} TopPlayer;

/**
 * @brief Initialization function
 * @param argc Number of arguments passed to the main function
 * @param argv Arguments passed to the main function
 */
void MB_init(int argc, char** argv);

/**
 * @brief Shutdown function to release all resources before closing
 */
void MB_shutdown();

#endif
