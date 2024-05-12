#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "../config.h"
#include "../init/init.h"
#include "../game/game.h"
#include "../scene/scene.h"
#include "../draw/draw.h"
#include "../dynamic_array/dynamic_array.h"


extern SDL_Renderer* renderer;

extern SDL_TimerID timerModf;

extern TTF_Font *fontMain;
extern TTF_Font *fontGameLabel;
extern TTF_Font *fontSelectLevel;

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
extern char gameLabelText[];

extern TopPlayer topPlayers[];

extern float logoYPos;
extern float backgroundColor;

extern enum GameScene gameScene;
extern enum BallStatus ballStatus;
extern enum PlatformStatus platformStatus;

void MB_drawSceneGame() {    
    if (platformStatus == PLATFORM_MOVE_LEFT) {
        platformRect.x -= 3;
        if (platformRect.x < FIELD_LEFT_SIDE) {
            platformRect.x = FIELD_LEFT_SIDE;
        }
    }
    else if (platformStatus == PLATFORM_MOVE_RIGHT) {
        platformRect.x += 3;
        if ((platformRect.x + platformRect.w) > FIELD_LEFT_SIDE+FIELD_WIDTH) {
            platformRect.x = FIELD_LEFT_SIDE+FIELD_WIDTH-platformRect.w;
        }
    }

    // Поле
    MB_drawRect(renderer, (SDL_Rect){FIELD_LEFT_SIDE-FIELD_OUTLINE, FIELD_TOP_SIDE-FIELD_OUTLINE, FIELD_OUTLINE, FIELD_HEIGHT+FIELD_OUTLINE},(SDL_Color){180, 180, 180, 255});
    MB_drawRect(renderer, (SDL_Rect){FIELD_LEFT_SIDE-FIELD_OUTLINE, FIELD_TOP_SIDE-FIELD_OUTLINE, FIELD_WIDTH+FIELD_OUTLINE, FIELD_OUTLINE},(SDL_Color){180, 180, 180, 255});
    MB_drawRect(renderer, (SDL_Rect){FIELD_LEFT_SIDE+FIELD_WIDTH, FIELD_TOP_SIDE-FIELD_OUTLINE, FIELD_OUTLINE, FIELD_HEIGHT+FIELD_OUTLINE},(SDL_Color){180, 180, 180, 255});

    sprintf(scoreText, "Mana:%d", score);
    MB_drawBordererText(renderer, scoreText, fontMain, FIELD_LEFT_SIDE, FIELD_TOP_SIDE-10, "left", "bottom", (SDL_Color){255,255,255,255},2);

    for (int i = 0; i < lifes; i++) {
        MB_drawImage(renderer, heartTexture,
        (SDL_Rect) {FIELD_LEFT_SIDE + FIELD_WIDTH - HEART_SIZE - (i*(HEART_SIZE+4)), FIELD_TOP_SIDE-10-HEART_SIZE, HEART_SIZE, HEART_SIZE},
        (SDL_Color) {255, 255, 255, 255});
    }

    MB_drawRect(renderer, platformRect, (SDL_Color){210, 210, 210, 255});

    if (ballStatus != BALL_STOPPED) {
        double radians = ballDirectionAngle * PI / 180.0;
        double x = cos(radians)*2;
        double y = sin(radians)*2;
        circleRect.x += x;
        circleRect.y -= y;
        
        if (circleRect.x <= FIELD_LEFT_SIDE || circleRect.x + circleRect.w >= FIELD_RIGHT_SIDE) {
            ballDirectionAngle = 180 - ballDirectionAngle;
            if (circleRect.x <= FIELD_LEFT_SIDE) {
                circleRect.x = FIELD_LEFT_SIDE + 1;
            } else if (circleRect.x + circleRect.w >= FIELD_RIGHT_SIDE) {
                circleRect.x = FIELD_RIGHT_SIDE - circleRect.w - 1;
            }
        }

        if (circleRect.y < FIELD_TOP_SIDE) {
            ballStatus = BALL_MOVE_DOWN;
            ballDirectionAngle = 360 - ballDirectionAngle;
        }

        if (circleRect.y + circleRect.h > platformRect.y && circleRect.y + circleRect.h/2 < platformRect.y && circleRect.x + circleRect.w > platformRect.x && circleRect.x < platformRect.x + platformRect.w) {
            ballStatus = BALL_MOVE_UP;
            circleRect.y = platformRect.y - circleRect.h - 1;
            ballDirectionAngle = 360 - ballDirectionAngle + 0.5f * (platformRect.x + platformRect.w/2 - circleRect.x + circleRect.w/2);
        }

        if (circleRect.y > WINDOW_HEIGHT) {
            lifes--;
            score -= 500;
            if (score < 0) {
                score = 0;
            }
            ballStatus = BALL_STOPPED;
            platformStatus = PLATFORM_STOPPED;
            platformRect.x = FIELD_LEFT_SIDE + (FIELD_WIDTH-PLATFORM_WIDTH) / 2;
            circleRect.x = platformRect.x + platformRect.w/2 - circleRect.w/2;
            circleRect.y = platformRect.y - circleRect.h;
            if (lifes <= 0 && gameScene != SCENE_GAME_OVER) {
                onGameOver(score, SCENE_GAME_OVER);
                fadeMusic(false);
                return;
            }
        }
    }

    int brickCount = 0;

    for (int i = 0; i < field.size; i++) {
        Brick* brick = (Brick*)field.items[i];
        rectBrick.x = FIELD_LEFT_SIDE + brick->posX*BRICK_WIDTH;
        rectBrick.y = FIELD_TOP_SIDE + brick->posY*BRICK_HEIGHT;
        if (circleRect.x <= rectBrick.x + BRICK_WIDTH &&
            circleRect.x + circleRect.w >= rectBrick.x &&
            circleRect.y <= rectBrick.y + BRICK_HEIGHT &&
            circleRect.y + circleRect.h >= rectBrick.y &&
            brick->color != ' ') {


            float ymin = (rectBrick.y > circleRect.y) ? rectBrick.y : circleRect.y;
            float ymax = (rectBrick.y + FIELD_TOP_SIDE < circleRect.y + circleRect.h) ? rectBrick.y + FIELD_TOP_SIDE : circleRect.y + circleRect.h;
            float ysize = ymax - ymin;

            float xmin = (rectBrick.x > circleRect.x) ? rectBrick.x : circleRect.x;
            float xmax = (rectBrick.x + FIELD_LEFT_SIDE < circleRect.x + circleRect.w) ? rectBrick.x + FIELD_LEFT_SIDE : circleRect.x + circleRect.w;
            float xsize = xmax - xmin;

            if (xsize >= ysize) {
                circleRect.x += 1;
                if (circleRect.y + circleRect.h <= rectBrick.y) {
                    // Bottom
                    circleRect.y -= 5;
                } else {
                    // Top
                    circleRect.y += 5;
                }
            } else if (xsize < ysize) {
                if (circleRect.x + circleRect.w <= rectBrick.x) {
                    // Left
                    circleRect.x -= 5;
                } else {
                    // Right
                    circleRect.x += 5;
                }
            }

            switch (brick->color)
            {
                case '3':
                    brick->color = '2';
                    break;
                case '2':
                    brick->color = '1';
                    break;
                default:
                    brick->color = ' ';
                    break;
            }
            score += 50*scoreModf;
            SDL_RemoveTimer(timerModf);
            timerModf = SDL_AddTimer(1000, callbackModfTimer, NULL);
            scoreModf++;

            if (circleRect.x+circleRect.w > rectBrick.x && circleRect.x < rectBrick.x+rectBrick.w && (circleRect.y >= rectBrick.y+rectBrick.h-3 || circleRect.y+circleRect.h-3 <= rectBrick.y)) {
                ballDirectionAngle = 360 - ballDirectionAngle;
                if (ballStatus == BALL_MOVE_UP) {
                    ballStatus = BALL_MOVE_DOWN;
                }
                else if (ballStatus == BALL_MOVE_DOWN) {
                    ballStatus = BALL_MOVE_UP;
                }
            }
            else if (BALL_MOVE_UP) {
                ballDirectionAngle = 180 - ballDirectionAngle;
            }
            else {
                ballDirectionAngle = 360 - ballDirectionAngle;
                if (ballStatus == BALL_MOVE_UP) {
                    ballStatus = BALL_MOVE_DOWN;
                }
                else if (ballStatus == BALL_MOVE_DOWN) {
                    ballStatus = BALL_MOVE_UP;
                }
            }

        }

        if (brick->color == ' ') {
            dynarray_remove(&field, brick);
            continue;
        } 
        brickCount++;
        MB_drawImage(renderer, brickTexture, rectBrick, getBrickColorByIndex(brick->color));
    }
    
    if (brickCount == 0) {
        onGameOver(score + 2000, SCENE_GAME_WIN);
        fadeMusic(false);
    }

    MB_drawImage(renderer, circleTexture, (SDL_Rect) {circleRect.x, circleRect.y, circleRect.w, circleRect.h}, (SDL_Color) {255, 255, 255, 255});
}

void MB_drawSceneMainMenu() {
    logoYPos += logoDirection * 0.25f;
    if (logoYPos > 100 || logoYPos < 50) {
        logoDirection = -logoDirection;
    }
    MB_drawImage(renderer, logoTexture, (SDL_Rect) {(WINDOW_WIDTH-LOGO_WIDTH)/2, logoYPos, LOGO_WIDTH, LOGO_HEIGHT}, (SDL_Color) {255,255,255,255});
    
    SDL_Color color = (SDL_Color) {255, 255, 255, 255};
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= rectButtonStart.x && mouseX <= (rectButtonStart.x + rectButtonStart.w) && mouseY >= rectButtonStart.y && mouseY <= (rectButtonStart.y + rectButtonStart.h)) {
        color = (SDL_Color) {200, 200, 200, 255};
    }

    MB_drawImage(renderer, buttonTexture, rectButtonStart, color);
    MB_drawBordererText(renderer, "PLAY", fontMain, WINDOW_WIDTH - 50 - BUTTON_WIDTH*4/2, WINDOW_HEIGHT - 50 - BUTTON_HEIGHT*4/2, "center", "center", color, 2);

    MB_drawImage(renderer, top5Texture, (SDL_Rect){50, 200, TOP5_WIDTH, TOP5_HEIGHT}, (SDL_Color){255,255,255,255});
    for (int i = 1; i <= 5; i++) {
        char text[51];
        if (i <= topPlayersCount) {
            sprintf(text, "%d. %s - %d", i, topPlayers[i-1].name, topPlayers[i-1].score);
        }
        else {
            sprintf(text, "%d.", i);
        }
        MB_drawBordererText(renderer, text, fontMain, 50, 200 + TOP5_HEIGHT + 20 + 50*(i-1), "left", "top", (SDL_Color){255,255,255,255}, 2);
    }
}

void MB_drawSceneGameOver() {
    MB_drawBordererText(renderer, "GAME OVER", fontGameLabel, WINDOW_WIDTH/2, WINDOW_HEIGHT/2-150, "center", "center", (SDL_Color){254,174,34,255}, 2);
    MB_drawBordererText(renderer, gameLabelText, fontMain, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, "center", "center", (SDL_Color){255,255,255,255}, 2);

    SDL_Color color = (SDL_Color) {255, 255, 255, 255};
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= rectGameOverMainMenu.x && mouseX <= (rectGameOverMainMenu.x + rectGameOverMainMenu.w) && mouseY >= rectGameOverMainMenu.y && mouseY <= (rectGameOverMainMenu.y + rectGameOverMainMenu.h)) {
        color = (SDL_Color) {200, 200, 200, 255};
    }
    MB_drawImage(renderer, buttonTexture, rectGameOverMainMenu, color);
    MB_drawBordererText(renderer, "MAIN MENU", fontMain, rectGameOverMainMenu.x + rectGameOverMainMenu.w/2, rectGameOverMainMenu.y + rectGameOverMainMenu.h/2, "center", "center", color, 2);
}

void MB_drawSceneGameWin() {
    MB_drawBordererText(renderer, "GAME WIN", fontGameLabel, WINDOW_WIDTH/2, WINDOW_HEIGHT/2-150, "center", "center", (SDL_Color){254,174,34,255}, 2);
    MB_drawBordererText(renderer, gameLabelText, fontMain, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, "center", "center", (SDL_Color){255,255,255,255}, 2);

    SDL_Color color = (SDL_Color) {255, 255, 255, 255};
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= rectGameOverMainMenu.x && mouseX <= (rectGameOverMainMenu.x + rectGameOverMainMenu.w) && mouseY >= rectGameOverMainMenu.y && mouseY <= (rectGameOverMainMenu.y + rectGameOverMainMenu.h)) {
        color = (SDL_Color) {200, 200, 200, 255};
    }
    MB_drawImage(renderer, buttonTexture, rectGameOverMainMenu, color);
    MB_drawBordererText(renderer, "MAIN MENU", fontMain, rectGameOverMainMenu.x + rectGameOverMainMenu.w/2, rectGameOverMainMenu.y + rectGameOverMainMenu.h/2, "center", "center", color, 2);
}

void MB_drawSceneSelectLevel() {
    MB_drawBordererText(renderer, "Select difficulty level", fontSelectLevel, WINDOW_WIDTH/2, WINDOW_HEIGHT/2-250, "center", "center", (SDL_Color){254,174,34,255}, 2);

    SDL_Color color = (SDL_Color) {255, 255, 255, 255};
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= st1Rect.x && mouseX <= (st1Rect.x + st1Rect.w) && mouseY >= st1Rect.y && mouseY <= (st1Rect.y + st1Rect.h)) {
        color = (SDL_Color) {200, 200, 200, 255};
    }

    MB_drawImage(renderer, frameTexture, (SDL_Rect) st1Rect, getBrickColorByIndex('1'));
    MB_drawImage(renderer, st1Texture, st1Rect, color);
    MB_drawBordererText(renderer, "Junior", fontMain, st1Rect.x+st1Rect.w/2, st1Rect.y+st1Rect.h-27, "center", "center", color, 2);

    color = (SDL_Color) {255, 255, 255, 255};
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= st2Rect.x && mouseX <= (st2Rect.x + st2Rect.w) && mouseY >= st2Rect.y && mouseY <= (st2Rect.y + st2Rect.h)) {
        color = (SDL_Color) {200, 200, 200, 255};
    }
    MB_drawImage(renderer, frameTexture, (SDL_Rect) st2Rect, getBrickColorByIndex('2'));
    MB_drawImage(renderer, st2Texture, st2Rect, color);
    MB_drawBordererText(renderer, "Middle", fontMain, st2Rect.x+st2Rect.w/2, st2Rect.y+st2Rect.h-27, "center", "center", color, 2);

    color = (SDL_Color) {255, 255, 255, 255};
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= st3Rect.x && mouseX <= (st3Rect.x + st3Rect.w) && mouseY >= st3Rect.y && mouseY <= (st3Rect.y + st3Rect.h)) {
        color = (SDL_Color) {200, 200, 200, 255};
    }
    MB_drawImage(renderer, frameTexture, (SDL_Rect) st3Rect, getBrickColorByIndex('3'));
    MB_drawImage(renderer, st3Texture, st3Rect, color);
    MB_drawBordererText(renderer, "Senior", fontMain, st3Rect.x+st3Rect.w/2, st3Rect.y+st3Rect.h-27, "center", "center", color, 2);

    color = (SDL_Color) {255, 255, 255, 255};
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= selectLevelBackButtonRect.x && mouseX <= (selectLevelBackButtonRect.x + selectLevelBackButtonRect.w) && mouseY >= selectLevelBackButtonRect.y && mouseY <= (selectLevelBackButtonRect.y + selectLevelBackButtonRect.h)) {
        color = (SDL_Color) {200, 200, 200, 255};
    }
    MB_drawImage(renderer, buttonTexture, selectLevelBackButtonRect, color);
    MB_drawBordererText(renderer, "BACK", fontMain, selectLevelBackButtonRect.x + selectLevelBackButtonRect.w/2, selectLevelBackButtonRect.y + selectLevelBackButtonRect.h/2, "center", "center", color, 2);
}

void MB_drawScene(enum GameScene scene) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    MB_drawImage(renderer, backgroundTexture, (SDL_Rect){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, (SDL_Color){backgroundColor, backgroundColor, backgroundColor, 255});
    switch (scene)
    {
        case SCENE_GAME:
            MB_drawSceneGame();
            break;
        case SCENE_MAIN_MENU:
            MB_drawSceneMainMenu();
            break;
        case SCENE_SELECT_LEVEL:
            MB_drawSceneSelectLevel();
            break;
        case SCENE_GAME_WIN:
            MB_drawSceneGameWin();
            break;
        case SCENE_GAME_OVER:
            MB_drawSceneGameOver();
            break;
    }
}