#include "../config.h"
#include "../init/init.h"
#include "../game/game.h"
#include "../scene/scene.h"
#include "../dynamic_array/dynamic_array.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern SDL_TimerID timerModf;
extern SDL_TimerID timerID;
extern SDL_TimerID timerBackground;

extern TTF_Font *fontMain;
extern TTF_Font *fontGameLabel;
extern TTF_Font *fontSelectLevel;

extern Mix_Music* backgroundMusic;
extern Mix_Chunk* winSoundEffect;
extern Mix_Chunk* overSoundEffect;

extern dynarray field;

extern SDL_Rect platformRect;
extern SDL_Rect rectBrick;
extern SDL_Rect rectButtonStart;
extern SDL_Rect st1Rect;
extern SDL_Rect st2Rect;
extern SDL_Rect st3Rect;
extern SDL_Rect selectLevelBackButtonRect;
extern SDL_Rect rectGameOverMainMenu;

extern MB_Rect circleRect;

extern SDL_Texture* backgroundTexture;
extern SDL_Texture* brickTexture;
extern SDL_Texture* circleTexture;
extern SDL_Texture* heartTexture;
extern SDL_Texture* logoTexture;
extern SDL_Texture* buttonTexture;
extern SDL_Texture* top5Texture;
extern SDL_Texture* frameTexture;
extern SDL_Texture* st1Texture;
extern SDL_Texture* st2Texture;
extern SDL_Texture* st3Texture;

extern int ballDirectionAngle;
extern int topPlayersCount;
extern int scoreModf;
extern int score;
extern int lifes;
extern int logoDirection;

extern char scoreText[];
extern char playerName[];
extern char gameLabelText[];

extern SDL_Event event;

extern bool quit;

extern TopPlayer topPlayers[];

extern float logoYPos;
extern float backgroundColor;

extern FILE* fileRecords;

extern enum GameScene gameScene;
extern enum BallStatus ballStatus;
extern enum PlatformStatus platformStatus;

void MB_start() {
    timerBackground = SDL_AddTimer(10, callbackBackgroundTimer, NULL);
    refreshTop5Players();
    Mix_PlayMusic(backgroundMusic, -1);
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
                    if (platformStatus != PLATFORM_BOTH) {
                        if (platformStatus == PLATFORM_MOVE_RIGHT) {
                            platformStatus = PLATFORM_BOTH;
                        }
                        else {
                            platformStatus = PLATFORM_MOVE_LEFT;
                        }
                    }
                    if (ballStatus == BALL_STOPPED) {
                        ballDirectionAngle = 110;
                    }
                    ballStatus = BALL_MOVE_UP;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
                    if (platformStatus != PLATFORM_BOTH) {
                        if (platformStatus == PLATFORM_MOVE_LEFT) {
                            platformStatus = PLATFORM_BOTH;
                        }
                        else {
                            platformStatus = PLATFORM_MOVE_RIGHT;
                        }
                    }
                    if (ballStatus == BALL_STOPPED) {
                        ballDirectionAngle = 70;
                    }
                    ballStatus = BALL_MOVE_UP;
                }
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
                    if (platformStatus == PLATFORM_BOTH) {
                        platformStatus = PLATFORM_MOVE_RIGHT;
                    }
                    else {
                        platformStatus = PLATFORM_STOPPED;
                    }
                }
                else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
                    if (platformStatus == PLATFORM_BOTH) {
                        platformStatus = PLATFORM_MOVE_LEFT;
                    }
                    else {
                        platformStatus = PLATFORM_STOPPED;
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    switch (gameScene)
                    {
                        case SCENE_MAIN_MENU:
                            if (event.button.x >= rectButtonStart.x && event.button.x <= (rectButtonStart.x + rectButtonStart.w) && event.button.y >= rectButtonStart.y && event.button.y <= (rectButtonStart.y + rectButtonStart.h)) {
                                gameScene = SCENE_SELECT_LEVEL;
                            }
                            break;
                        case SCENE_SELECT_LEVEL:
                            if (event.button.x >= selectLevelBackButtonRect.x && event.button.x <= (selectLevelBackButtonRect.x + selectLevelBackButtonRect.w) && event.button.y >= selectLevelBackButtonRect.y && event.button.y <= (selectLevelBackButtonRect.y + selectLevelBackButtonRect.h)) {
                                refreshTop5Players();
                                gameScene = SCENE_MAIN_MENU;
                            }
                            else if (event.button.x >= st1Rect.x && event.button.x <= (st1Rect.x + st1Rect.w) && event.button.y >= st1Rect.y && event.button.y <= (st1Rect.y + st1Rect.h)) {
                                loadLevel(&field, "junior");
                                refreshGameToDefault();
                            }
                            else if (event.button.x >= st2Rect.x && event.button.x <= (st2Rect.x + st2Rect.w) && event.button.y >= st2Rect.y && event.button.y <= (st2Rect.y + st2Rect.h)) {
                                loadLevel(&field, "middle");
                                refreshGameToDefault();
                            }
                            else if (event.button.x >= st3Rect.x && event.button.x <= (st3Rect.x + st3Rect.w) && event.button.y >= st3Rect.y && event.button.y <= (st3Rect.y + st3Rect.h)) {
                                loadLevel(&field, "senior");
                                refreshGameToDefault();
                            }
                            break;
                        case SCENE_GAME_WIN:
                            if (event.button.x >= rectGameOverMainMenu.x && event.button.x <= (rectGameOverMainMenu.x + rectGameOverMainMenu.w) && event.button.y >= rectGameOverMainMenu.y && event.button.y <= (rectGameOverMainMenu.y + rectGameOverMainMenu.h)) {
                                refreshTop5Players();
                                fadeMusic(true);
                                gameScene = SCENE_MAIN_MENU;
                            }
                            break;
                        case SCENE_GAME_OVER:
                            if (event.button.x >= rectGameOverMainMenu.x && event.button.x <= (rectGameOverMainMenu.x + rectGameOverMainMenu.w) && event.button.y >= rectGameOverMainMenu.y && event.button.y <= (rectGameOverMainMenu.y + rectGameOverMainMenu.h)) {
                                refreshTop5Players();
                                fadeMusic(true);
                                gameScene = SCENE_MAIN_MENU;
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        MB_drawScene(gameScene);
        SDL_RenderPresent(renderer);
    }
}

void loadLevel(dynarray *field, char* id) {
    
    char filePath[51] = "";
    sprintf(filePath, "assets/levels/%s.txt", id);
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n",filePath);
        return;
    }
    char str[101] = "";
    int k = 0;
    while (fgets(str, sizeof(str), file) != NULL) {
        for (size_t i = 0; i < strlen(str); i++) {
            if (str[i] != '\0' && str[i] != '\n') {
                Brick brick = {i,k,str[i]};
                Brick* p_brick = (Brick*)malloc(sizeof(Brick));
                *p_brick = brick;
                dynarray_push(field, (array_type)p_brick);
            }
        }
        k++;
    }
    fclose(file);
}

SDL_Color getBrickColorByIndex(char index) {
    switch (index)
    {
        case '1':
            return (SDL_Color) {0, 171, 240, 255};
        case '2':
            return (SDL_Color) {240, 164, 0, 255};
        case '3':
            return (SDL_Color) {240, 40, 0, 255};
        default:
            return (SDL_Color) {255, 255, 255, 255};
    }
}

Uint32 callbackScoreTimer(Uint32 interval, void* par __attribute__((unused))) {
    if (gameScene != SCENE_GAME) {
        return 0;
    }

    if (score >= 3) {
        score -= 3;
    }

    return interval;
}

Uint32 callbackModfTimer(Uint32 interval __attribute__((unused)), void* par __attribute__((unused))) {
    scoreModf = 1;
       
    return 0;
}

Uint32 callbackBackgroundTimer(Uint32 interval, void* par __attribute__((unused))) {
    switch (gameScene)
    {
        case SCENE_GAME:
            backgroundColor -= 5;
            if (backgroundColor < 20) {
                backgroundColor = 20;
                return 0;
            }
            break;
        default:
            backgroundColor += 5;
            if (backgroundColor > 255) {
                backgroundColor = 255;
                return 0;
            }
            break;
    }
       
    return interval;
}

void onGameOver(int score, enum GameScene scene) {
    gameScene = scene;
    switch (scene)
    {
        case SCENE_GAME_WIN:
            Mix_PlayChannel(-1, winSoundEffect, 0);
            break;
        case SCENE_GAME_OVER:
            Mix_PlayChannel(-1, overSoundEffect, 0);
            break;    
        default:
            break;
    }
    timerBackground = SDL_AddTimer(10, callbackBackgroundTimer, NULL);
    dynarray_clean(&field);
    sprintf(gameLabelText, "You received %d mana points", score);
    fileRecords = fopen("records.txt", "a");
    if (fileRecords == NULL) {
        printf("Error: failed to open records file");
        return;
    }
    fprintf(fileRecords, "%d,%s\n", score, playerName);
    fclose(fileRecords);
}

int countLines(FILE *file) {
    int count = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    if (ch != '\n' && count > 0) {
        count++;
    }

    rewind(file);

    return count;
}

int comparePlayers(const void *a, const void *b) {
    return ((TopPlayer *)b)->score - ((TopPlayer *)a)->score;
}

void refreshTop5Players() {
    fileRecords = fopen("records.txt", "r");
    if (fileRecords == NULL) {
        return;
    }

    int numLines = countLines(fileRecords) - 1;

    fseek(fileRecords, 0, SEEK_SET);

    TopPlayer *allPlayers = (TopPlayer *)malloc(numLines * sizeof(TopPlayer));
    for (int i = 0; i < numLines; i++) {
        strcpy(allPlayers[i].name, "");
        allPlayers[i].score = 0;
    }

    char line[32];
    topPlayersCount = 0;

    while (fgets(line, sizeof(line), fileRecords) != NULL) {
        // Разбиваем строку на очки и имя
        char *token = strtok(line, ",");
        if (token != NULL) {
            allPlayers[topPlayersCount].score = atoi(token);

            token = strtok(NULL, ",");
            if (token != NULL) {
                // Удаляем символ новой строки из имени
                token[strcspn(token, "\n")] = '\0';
                strncpy(allPlayers[topPlayersCount].name, token, sizeof(allPlayers[topPlayersCount].name));
            }
            topPlayersCount++;
        }
    }

    qsort(allPlayers, topPlayersCount, sizeof(TopPlayer), comparePlayers);

    if (topPlayersCount > 5) {
        topPlayersCount = 5;
    }
    for (int i = 0; i < topPlayersCount; i++) {
        strcpy(topPlayers[i].name, allPlayers[i].name);
        topPlayers[i].score = allPlayers[i].score;
    }

    free(allPlayers);
    fclose(fileRecords);
}

void refreshGameToDefault() {
    score = 1000;
    lifes = 3;
    scoreModf = 1;
    ballStatus = BALL_STOPPED;
    platformStatus = PLATFORM_STOPPED;
    platformRect.x = FIELD_LEFT_SIDE + (FIELD_WIDTH-PLATFORM_WIDTH) / 2;
    circleRect.x = platformRect.x + platformRect.w/2 - circleRect.w/2;
    circleRect.y = platformRect.y - circleRect.h;
    // Создание таймера, который каждые 2 секунды вызывает функцию callbackScoreTimer
    timerID = SDL_AddTimer(2000, callbackScoreTimer, NULL);
    gameScene = SCENE_GAME;
    timerBackground = SDL_AddTimer(10, callbackBackgroundTimer, NULL);
}


bool fadeMusicState = true;
float musicVolume = 1.0f;
Uint32 callbackFadeMusic(Uint32 interval, void* par __attribute__((unused))) {
    switch (fadeMusicState)
    {
        case true:
            musicVolume += 0.01f;
            if (musicVolume > 1.0f) {
                musicVolume = 1.0f;
                Mix_VolumeMusic(MIX_MAX_VOLUME * musicVolume);
                return 0;
            }
            break;
        
        default:
            musicVolume -= 0.01f;
            if (musicVolume < 0.1f) {
                musicVolume = 0.1f;
                Mix_VolumeMusic(MIX_MAX_VOLUME * musicVolume);
                return 0;
            }
            break;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME * musicVolume);

    return interval;
}

void fadeMusic(bool flag) {
    fadeMusicState = flag;
    SDL_AddTimer(10, callbackFadeMusic, NULL);
}