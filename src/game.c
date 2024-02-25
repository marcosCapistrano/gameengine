#include "game.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../libs/hmm/handmademath.h"

#define T Game_T
struct T
{
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    uint32_t millisPrevFrame;
};

void processInput(T game)
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            game->isRunning = false;
            break;

        case SDL_KEYDOWN:
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                game->isRunning = false;
            }
            break;
        }
    }
}

T Game_new()
{
    T game;
    game = malloc(sizeof game);
    if (!game)
    {
        printf("malloc failed");
        return NULL;
    }

    return game;
}

void Game_init(T game)
{
    game->isRunning = false;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    if (TTF_Init() != 0)
    {
        SDL_Log("Unable to initialize SDL_ttf: %s", SDL_GetError());
        return;
    }

    SDL_Window *window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        SDL_Log("Unable to create SDL window: %s", SDL_GetError());
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!window)
    {
        SDL_Log("Unable to create SDL renderer: %s", SDL_GetError());
        return;
    }

    // Load a font
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24);
    if (!font)
    {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return;
    }

    game->window = window;
    game->renderer = renderer;
    game->font = font;
    game->millisPrevFrame = 0;
    game->isRunning = true;
}

Vec2 playerPosition;
Vec2 playerVelocity;
void setup()
{
    playerPosition = V2(10.0, 10.0);
    playerVelocity = V2(100.0, 50.0);
}

void update(T game)
{
    // If we are running too fast, waste some time until we reach GAME_MILLIS_PER_FRAME
    uint32_t timeToWait = GAME_MILLIS_PER_FRAME - (SDL_GetTicks() - game->millisPrevFrame);
    if (timeToWait > 0 && timeToWait <= GAME_MILLIS_PER_FRAME)
    {
        SDL_Delay(timeToWait);
    }
    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - game->millisPrevFrame) / 1000.0;

    playerPosition = AddV2(playerPosition, MulV2F(playerVelocity, deltaTime));
}

void render(T game)
{
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);

    SDL_Rect player = {playerPosition.X, playerPosition.Y, 20, 20};
    SDL_RenderFillRect(game->renderer, &player);

    // Render text to a surface
    SDL_Color textColor = {255, 255, 255, 0}; // white

    //TODO: move this whole block to update, and save deltaTime in game object
    /* ----------------- */
    double deltaTime = (SDL_GetTicks() - game->millisPrevFrame) / 1000.0;
    char fpsString[400];
    sprintf(fpsString, "FPS: %f", 1/deltaTime);

    game->millisPrevFrame = SDL_GetTicks();
    /* ----------------- */

    SDL_Surface *surface = TTF_RenderText_Solid(game->font, fpsString, textColor);
    if (!surface)
    {
        SDL_Log("Failed to create surface: %s", TTF_GetError());
        return 1;
    }

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    if (!texture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
    SDL_FreeSurface(surface); // No longer need the surface

    // Copy the texture to the renderer
    SDL_Rect fpsArea = {
        10, 10, 100, 50
    };
    SDL_RenderCopy(game->renderer, texture, NULL, &fpsArea);
    // Cleanup
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(game->renderer);
}

void Game_run(T game)
{
    setup();
    while (game->isRunning)
    {
        processInput(game);
        update(game);
        render(game);
    }
}

void Game_destroy(T game)
{
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);

    SDL_Quit();
}