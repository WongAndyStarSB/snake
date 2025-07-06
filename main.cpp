#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>

#include "Logger.hpp"
#include "utils.hpp"
#include "Fraction.hpp"
#include "Vector2D.hpp"
#include "Matrix.hpp"
#include "Pos2D.hpp"
#include "Size2D.hpp"
#include "Game.hpp"
#include "GameBoardObjects.hpp"

#include "Level.hpp"
#include "levels.hpp"

using namespace snake;

bool game_running = false;
bool player_started = false;
const int FRAME_RATE = 300;
const Fraction MILLIS_PER_FRAME_FRACTION = Fraction(1000, FRAME_RATE); // 1000 milliseconds in a second divided by frame rate
const int MILLIS_PER_FRAME = MILLIS_PER_FRAME_FRACTION.floor(); // Convert to milliseconds
Vector2D player_direction(0, 0);


int frame_num = 0;

SDL_Window* window;
SDL_Renderer* renderer;

int main() {
    
    SDL_CreateWindowAndRenderer("Snake", 100, 100, NULL, &window, &renderer);
    SDL_Event event;
    
    srand(1);
    levels::init_testing_levels();
    levels::init_levels(); // Initialize levels

    const Level& lev = Level::find_level("0001");
    Game game(lev);
    game.init();
    game.start();
    
    Utils::clear_terminal();
    Utils::delay_for_time_in_ms(1000);

    game_running = true;
    while (game_running) {
        
        if (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				game_running = false;
				break;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                player_started = true;
                if (event.key.scancode == SDL_SCANCODE_W || event.key.scancode == SDL_SCANCODE_UP || event.key.key == SDLK_KP_8) {
                    player_direction = Vector2D::get_up_vector();
                } else if (event.key.scancode == SDL_SCANCODE_S || event.key.scancode == SDL_SCANCODE_DOWN || event.key.key == SDLK_KP_5) {
                    player_direction = Vector2D::get_down_vector();
                } else if (event.key.scancode == SDL_SCANCODE_A || event.key.scancode == SDL_SCANCODE_LEFT || event.key.key == SDLK_KP_4) {
                    player_direction = Vector2D::get_left_vector();
                } else if (event.key.scancode == SDL_SCANCODE_D || event.key.scancode == SDL_SCANCODE_RIGHT || event.key.key == SDLK_KP_6) {
                    player_direction = Vector2D::get_right_vector();
                } else if (event.key.scancode == SDL_SCANCODE_SPACE) {
                    if (game.get_status() == RUNNING) {
                        game.pause();
                    } else if (game.get_stop_reason() == PAUSED) {
                        game.resume();
                    }
                } else if (event.key.scancode == SDL_SCANCODE_R) {
                    if (game.get_status() == STOP && game.get_stop_reason() != PAUSED) {
                        player_direction = Vector2D::get_zero_vector();
                        game.restart();
                    }
                }
            }
        }
        game.update(frame_num, player_direction, std::cout);


        ++frame_num;
        SDL_Delay(MILLIS_PER_FRAME);
        //Utils::delay_for_time_in_ms(MILLIS_PER_FRAME);

        
    }
    return 0;
}

