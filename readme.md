# Breakout: MANABREAK!
This game is a remake of the cult classic Breakout. In this concept, you play as a wizard who has run out of mana (hence the name: Manabreak - a period in the game when the player runs out of mana, preventing the use of abilities). To replenish mana, the wizard engages in a simple Breakout mini-game. The game offers multiple difficulty levels, from Junior to Senior, based on the wizard's skill level.

## Used Libraries
Third-party libraries were utilized in the creation of this game. Before compiling the project, make sure to install the following libraries:
```
SDL2
SDL2_image
SDL2_ttf
SDL2_mixer
```

## Compilation
1. Open the terminal and navigate to the project folder:
```bash
cd /path/to/game/
```
2. Enter the compilation command:
```bash
gcc main.c init/init.c game/game.c dynamic_array/dynamic_array.c draw/draw.c scene/scene.c -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm -omanabreak
```

## Running the Game
1. Open the terminal and navigate to the project folder:
```bash
cd /path/to/game/
```
2. Enter the command to launch the game:
```bash
./manabreak nickname
```
where **nickname** is the player's name (up to 15 characters).

## Controls
The player controls the platform from which the ball bounces.  
→ (Right arrow) / D - move the platform to the right  
← (Left arrow) / A - move the platform to the left

## Recording Results
High scores are saved in the records.txt file. The top 5 players with the best scores are displayed in the main menu on the left side of the screen.

## Features
The game boasts a pleasant interface, smooth gameplay, and sound and music effects for player comfort.  
Enjoy the game!