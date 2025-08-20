#include "Application.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <stdexcept>
#include <string>
#include <tuple>

#include "Logger.hpp"
#include "ColorConstants.hpp"

using namespace snake;


Application::Application() {
    init_sdl();
}
Application::~Application() {
    SDL_Quit();
}

void Application::add_screen(const std::string& name, std::unique_ptr<Screen> screen) {
    if (screens.find(name) != screens.end()) {
        log_and_throw<std::runtime_error>("add_screen", "Screen with name '" + name + "' already exists");
    }
    screens[name] = std::move(screen);
}
void Application::switch_to(const std::string& name) {
    if (screens.find(name) == screens.end()) {
        log_and_throw<std::runtime_error>("switch_to", "Screen with name '" + name + "' does not exists");
    }
    current_screen = screens[name].get(); // non-owning pointer to the screen
}
Screen* Application::get_current_screen() {
    if (!current_screen) {
        log_and_throw<std::runtime_error>("get_current_screen", "No current screen set");
    }
    return current_screen;
}

void Application::init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_and_throw<std::runtime_error>(
            "init_sdl", 
            "SDL could not initialize! SDL_Error: " + std::string(SDL_GetError())
        );
    }
    
    SDL_Window* raw_window = nullptr;
    SDL_Renderer* raw_renderer = nullptr;
    if (SDL_CreateWindowAndRenderer("Snake Game", 800, 600, NULL, &raw_window, &raw_renderer)) {
        log_and_throw<std::runtime_error>(
            "init_sdl", 
            "Window/Renderer could not be created! SDL_Error: " + std::string(SDL_GetError())
        );
    }
    window_ptr.reset(raw_window);
    renderer_ptr.reset(raw_renderer);
}

void Application::render() {
    SDL_Renderer* renderer = renderer_ptr.get();
    SDL_Window* window = window_ptr.get();
    
    if (!current_screen) {
        using namespace ColorConstants;
        SDL_SetRenderDrawColor(renderer, DARK_GREEN_RGBA[0], DARK_GREEN_RGBA[1], DARK_GREEN_RGBA[2], DARK_BLUE_RGBA[3]); // Set background color to dark_green
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        return;
    }

    current_screen->run();
    
    
}

// void Application::handle_events() {
//     SDL_Event event;
//     while (SDL_PollEvent(&event)) {
//         if (event.type == SDL_EVENT_QUIT) {
//             log("handle_events", "Quit event received", Logger::INFO);
//             throw std::runtime_error("Quit event received");
//         }
//         // Handle other events like key presses here
//     }
// }

void Application::window_deleter(SDL_Window* w) {
    if (w) SDL_DestroyWindow(w);
}
void Application::renderer_deleter(SDL_Renderer* w) {
    if (w) SDL_DestroyRenderer(w);
}


