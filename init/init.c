#include "../config.h"
#include "../init/init.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "../dynamic_array/dynamic_array.h"

SDL_Window* window;
SDL_Renderer* renderer;

SDL_TimerID timerModf;
SDL_TimerID timerID;
SDL_TimerID timerBackground;

TTF_Font *fontMain = NULL;
TTF_Font *fontGameLabel = NULL;
TTF_Font *fontSelectLevel = NULL;

Mix_Music* backgroundMusic = NULL;
Mix_Chunk* winSoundEffect = NULL;
Mix_Chunk* overSoundEffect = NULL;

dynarray field = {};

SDL_Rect platformRect;
SDL_Rect rectBrick;
SDL_Rect rectButtonStart;
SDL_Rect st1Rect;
SDL_Rect st2Rect;
SDL_Rect st3Rect;
SDL_Rect selectLevelBackButtonRect;
SDL_Rect rectGameOverMainMenu;

MB_Rect circleRect;

SDL_Texture* backgroundTexture;
SDL_Texture* brickTexture;
SDL_Texture* circleTexture;
SDL_Texture* heartTexture;
SDL_Texture* logoTexture;
SDL_Texture* buttonTexture;
SDL_Texture* top5Texture;
SDL_Texture* frameTexture;
SDL_Texture* st1Texture;
SDL_Texture* st2Texture;
SDL_Texture* st3Texture;

int ballDirectionAngle = 0;
int topPlayersCount = 0;
int scoreModf = 1;
int score = 1000;
int lifes = 3;
int logoDirection = 1;

char scoreText[20];
char playerName[16] = "";
char gameLabelText[64] = "";

SDL_Event event;

bool quit = false;

TopPlayer topPlayers[5];

float logoYPos = 100;
float backgroundColor = 255;

FILE* fileRecords;

enum GameScene gameScene = SCENE_MAIN_MENU;
enum BallStatus ballStatus = BALL_STOPPED;
enum PlatformStatus platformStatus = PLATFORM_STOPPED;


void MB_init(int argc, char** argv) {
    if (argc < 2) {
        strcpy(playerName,"Player");
    }
    else {
        strncpy(playerName, argv[1], 15);
        playerName[15] = '\0';
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
        fprintf(stderr, "SDL_GetDesktopDisplayMode Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow(
        "Breakout: MANABREAK!",
        (displayMode.w-WINDOW_WIDTH)/2,
        (displayMode.h-WINDOW_HEIGHT)/2,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (TTF_Init() != 0)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        fprintf(stderr, "TTF_Init Error: %s", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    fontGameLabel = TTF_OpenFont("assets/forque.ttf", 92);
    if (fontGameLabel == NULL) {
        fprintf(stderr, "TTF_OpenFont failed: %s\n", TTF_GetError());
    }
    fontSelectLevel = TTF_OpenFont("assets/forque.ttf", 64);
    if (fontSelectLevel == NULL) {
        fprintf(stderr, "TTF_OpenFont failed: %s\n", TTF_GetError());
    }
    fontMain = TTF_OpenFont("assets/forque.ttf", 32);
    if (fontGameLabel == NULL) {
        fprintf(stderr, "TTF_OpenFont failed: %s\n", TTF_GetError());
    }

    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        fprintf(stderr, "Mix_Init Error: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        fprintf(stderr, "Mix_OpenAudio Error: %s", SDL_GetError());
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    backgroundMusic = Mix_LoadMUS("assets/sfx/background_music.mp3");
    winSoundEffect = Mix_LoadWAV("assets/sfx/game_win.wav");
    Mix_VolumeChunk(winSoundEffect, MIX_MAX_VOLUME * 0.5f);
    overSoundEffect = Mix_LoadWAV("assets/sfx/game_over.wav");
    Mix_VolumeChunk(overSoundEffect, MIX_MAX_VOLUME * 0.3f);

    platformRect = (SDL_Rect) {
            FIELD_LEFT_SIDE + (FIELD_WIDTH-PLATFORM_WIDTH) / 2,
            WINDOW_HEIGHT-PLATFORM_OFFSET,
            PLATFORM_WIDTH,
            PLATFORM_HEIGHT
        };
    circleRect = (MB_Rect) {platformRect.x + platformRect.w/2 - BALL_SIZE/2, platformRect.y - BALL_SIZE, BALL_SIZE, BALL_SIZE};
    rectBrick = (SDL_Rect) {0, 0, BRICK_WIDTH, BRICK_HEIGHT};
    rectButtonStart = (SDL_Rect) {WINDOW_WIDTH - 50 - BUTTON_WIDTH*4, WINDOW_HEIGHT - 50 - BUTTON_HEIGHT*4, BUTTON_WIDTH*4, BUTTON_HEIGHT*4};
    SDL_Rect defRect = {(WINDOW_WIDTH-FRAME_WIDTH)/2,(WINDOW_HEIGHT-FRAME_HEIGHT)/2,FRAME_WIDTH,FRAME_HEIGHT};
    st1Rect = (SDL_Rect) {defRect.x - FRAME_WIDTH - 20, defRect.y, defRect.w, defRect.h};
    st2Rect = (SDL_Rect) {defRect.x, defRect.y, defRect.w, defRect.h};
    st3Rect = (SDL_Rect) {defRect.x + FRAME_WIDTH + 20, defRect.y, defRect.w, defRect.h};
    rectGameOverMainMenu = (SDL_Rect) {(WINDOW_WIDTH - BUTTON_WIDTH*4)/2, (WINDOW_HEIGHT - BUTTON_HEIGHT*4)/2 + 100, BUTTON_WIDTH*4, BUTTON_HEIGHT*4};
    selectLevelBackButtonRect = (SDL_Rect) {(WINDOW_WIDTH - BUTTON_WIDTH*4)/2, (WINDOW_HEIGHT - BUTTON_HEIGHT*4)/2 + 220, BUTTON_WIDTH*4, BUTTON_HEIGHT*4};

    SDL_Surface* surface = IMG_Load("assets/gfx/brick.png");
    brickTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/circle.png");
    circleTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/background.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/heart.png");
    heartTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/logo.png");
    logoTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/button.png");
    buttonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/top5.png");
    top5Texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/frame.png");
    frameTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/st1.png");
    st1Texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/st2.png");
    st2Texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/gfx/st3.png");
    st3Texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);


    dynarray_init(&field, 1);
}

void MB_shutdown() {
    dynarray_free(&field);

    if (brickTexture != NULL) {
        SDL_DestroyTexture(brickTexture);
    }
    if (circleTexture != NULL) {
        SDL_DestroyTexture(circleTexture);
    }
    if (backgroundTexture != NULL) {
        SDL_DestroyTexture(backgroundTexture);
    }
    if (heartTexture != NULL) {
        SDL_DestroyTexture(heartTexture);
    }
    if (logoTexture != NULL) {
        SDL_DestroyTexture(logoTexture);
    }
    if (buttonTexture != NULL) {
        SDL_DestroyTexture(buttonTexture);
    }
    if (top5Texture != NULL) {
        SDL_DestroyTexture(top5Texture);
    }
    if (frameTexture != NULL) {
        SDL_DestroyTexture(frameTexture);
    }
    if (st1Texture != NULL) {
        SDL_DestroyTexture(st1Texture);
    }
    if (st2Texture != NULL) {
        SDL_DestroyTexture(st2Texture);
    }
    if (st3Texture != NULL) {
        SDL_DestroyTexture(st3Texture);
    }

    if (fontMain != NULL) {
        TTF_CloseFont(fontMain);
    }
    if (fontGameLabel != NULL) {
        TTF_CloseFont(fontGameLabel);
    }
    if (fontSelectLevel != NULL) {
        TTF_CloseFont(fontSelectLevel);
    }

    TTF_Quit();
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(winSoundEffect);
    Mix_FreeChunk(overSoundEffect);
    Mix_Quit();

    SDL_RemoveTimer(timerID);
    SDL_RemoveTimer(timerModf);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); // Sometimes causes double freeing (reusing free(), I found only that it's because of this function, but didn't find how to fix it)
}