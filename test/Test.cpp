#include <SDL3/SDL.h>
#include <stdio.h>

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Failed to initialize SDL");
    }

    SDL_Window *window = SDL_CreateWindow("3DMath test", 800, 600, 0);

    SDL_Event event;
    bool running = true;

    while(running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                running = false;
                break;
            }
        }
    }
}