#include "init/init.h"
#include "game/game.h"

int main(int argc, char** argv)
{
    MB_init(argc, argv);
    MB_start();
    MB_shutdown();
    return 0;
}