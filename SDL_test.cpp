#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>


std::string direction = "UP";

int direction_user_should_move(const SDL_Event* e);
int temp;

SDL_Color window_color{0, 100, 200, 255}; // RGBA color

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Snake Game",800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    for (int i = 0; i < 1000; ++i) {
        window_color.r = (window_color.r + 1) % 256;
        window_color.g = (window_color.g + 1) % 256;
        window_color.b = (window_color.b + 1) % 256;
        SDL_SetRenderDrawColor(renderer, window_color.r, window_color.g, window_color.b, window_color.a);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
	return 0;
}

int direction_user_should_move(const SDL_Event* e)
{
    SDL_assert(e->type == SDL_EVENT_KEY_DOWN); /* just checking key presses here... */
    if (e->key.scancode == SDL_SCANCODE_W) {
        return 1;  /* pressed what would be "W" on a US QWERTY keyboard. Move forward! */
    }
    else if (e->key.scancode == SDL_SCANCODE_S) {
        return -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
    }

    /* (We aren't doing it here, but extra credit if your code sees both keys are pressed and cancel each other out!) */

    return 0;  /* wasn't key in W or S location, don't move. */
}