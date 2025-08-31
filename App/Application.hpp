#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <map>
#include <stdexcept>
#include <memory>

#include "Logger.hpp"
#include "Screen.hpp"

namespace snake {
    class Application {
    private:
        std::map<std::string, std::unique_ptr<Screen>> screens;
        Screen* current_screen = nullptr;

        void init_sdl();
        void render();
        // void handle_events();

        static void window_deleter(SDL_Window* w);
        static void renderer_deleter(SDL_Renderer* r);
        std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_ptr{nullptr, window_deleter};
        std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_ptr{nullptr, renderer_deleter};
        void log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) const {
            Logger::log("Application::" + where, message, level);
        }
        template <typename ExceptionType>
        [[noreturn]] inline void log_and_throw(const std::string& where, const std::string& message) {
            Logger::log_and_throw<ExceptionType>("Application::" + where, message);
        }

    public:
        
        
        explicit Application();

        void add_screen(const std::string& name, std::unique_ptr<Screen> screen);
        void switch_to(const std::string& name);
        Screen* get_current_screen();
        void run(); // delegates to current_screen->run()
        ~Application();

        void run();
    };

} // namespace snake


#endif // APPLICATION_HPP