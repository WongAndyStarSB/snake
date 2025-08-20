#ifndef BUTTONS_HPP
#define BUTTONS_HPP


#include <SDL3/SDL.h>
#include <string>
#include <stdexcept>
#include <memory>
#include "Logger.hpp"
#include "ColorConstants.hpp"
#include "Polygon.hpp"


namespace snake {
class Buttons {
private:
    SDL_Window* window; // non-owning // don't delete
    SDL_Renderer* renderer; // non-owning // don't delete

    void log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) const {
        Logger::log("Buttons::" + where, message, level);
    }
    template <typename ExceptionType>
    [[noreturn]] inline void log_and_throw(const std::string& where, const std::string& message) {
        Logger::log_and_throw<ExceptionType>("Buttons::" + where, message);
    }

public:
    explicit Buttons(SDL_Window* arg_window, SDL_Renderer* arg_renderer)
        : window(arg_window), renderer(arg_renderer) {
        if (!window || !renderer) {
            log_and_throw<std::logic_error>("Buttons constructor", "Window or Renderer is null");
        }
    }

};

}

#endif // BUTTONS_HPP