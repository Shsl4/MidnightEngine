#include "SDLEngine.h"
#include <SDL2/SDL.h>

int SDLEngine::init(int argc, char** argv)
{

    getLogger()->info("Initializing MidnightEngine...");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {

        getLogger()->fatal("Failed to initialize SDL! Error: {1} {2} {0}", SDL_GetError(), 5, 3);
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("MidnightEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_SWSURFACE | SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    bool shouldRun = true;
    SDL_Event event;

    double r = 0, g = 0, b = 0;
    double addr = 0.00002, addg = 0.00004, addb = 0.00006;

    while (shouldRun) {

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    shouldRun = false;
                }
                break;
            case SDL_QUIT:
                shouldRun = false;
                break;
            default:
                break;
            }

        }

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, (Uint8)(sin(r) * 255), (Uint8)(sin(g) * 255), (Uint8)(sin(b) * 255), 255);

        r += addr;
        g += addg;
        b += addb;

        if (r > 1 || r < 0) {
            addr *= -1;
        }

        if (g > 1 || g < 0) {
            addg *= -1;
        }

        if (b > 1 || b < 0) {
            addb *= -1;
        }

        SDL_RenderPresent(renderer);
        
    }

    getLogger()->info("Destroying engine...");

    SDL_Quit();

	return 0;
}

void SDLEngine::loop()
{
}
