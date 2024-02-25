#include <SDL2/SDL.h>
#include "game.h"

int main()
{

    Game_T game = Game_new();

    if (game != NULL)
    {
        Game_init(game);
        Game_run(game);
        Game_destroy(game);
    }

    return 0;
}