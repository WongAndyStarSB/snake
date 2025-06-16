#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>

#include "Pos2D.hpp"
#include "Size2D.hpp"


SDL_Color DARK_GREEN = {2, 100, 64, 255};
SDL_Color CYAN = {0, 255, 255};


SDL_Window* window;
SDL_Renderer* renderer;


SDL_FRect rect = {0, 0, 0, 0};


int main() {
    if (!SDL_CreateWindowAndRenderer("Snake", 800, 800, NULL, &window, &renderer)) {
        throw SDL_GetError();
    }
    
    rect = {100, 100, 200, 200};
    SDL_Event event;
    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.scancode == SDL_SCANCODE_W) {
                rect.y -= 50;
            } else if (event.key.scancode == SDL_SCANCODE_S) {
                rect.y += 50;
            } else if (event.key.scancode == SDL_SCANCODE_A) {
                rect.x -= 50;
            } else if (event.key.scancode == SDL_SCANCODE_D) {
                rect.x += 50;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, DARK_GREEN.r, DARK_GREEN.g, DARK_GREEN.b, DARK_GREEN.a);
        SDL_RenderClear(renderer);

        
        SDL_SetRenderDrawColor(renderer, CYAN.r, CYAN.g, CYAN.b, CYAN.a);
        SDL_RenderFillRect(renderer, &rect);

        // SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(300);
    }

    std::cout << "hello world";
}