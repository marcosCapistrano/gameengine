#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <SDL2/SDL.h>

#define GAME_FPS 80
#define GAME_MILLIS_PER_FRAME 1000U / GAME_FPS

#define T Game_T
typedef struct T *T;

extern T Game_new();
extern void Game_init(T);
extern void Game_run(T);
extern void Game_destroy(T);

#undef T
#endif