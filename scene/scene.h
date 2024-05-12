#ifndef SCENE_H
#define SCENE_H

/**
 * @brief Draws the game scene
 */
void MB_drawSceneGame();

/**
 * @brief Draws the main menu scene
 */
void MB_drawSceneMainMenu();

/**
 * @brief Draws the game over scene
 */
void MB_drawSceneGameOver();

/**
 * @brief Draws the game win scene
 */
void MB_drawSceneGameWin();

/**
 * @brief Draws the level difficulty selection scene
 */
void MB_drawSceneSelectLevel();

/**
 * @brief Draws a specific scene in the game
 * @param scene Scene identifier
 */
void MB_drawScene(enum GameScene scene);

#endif
