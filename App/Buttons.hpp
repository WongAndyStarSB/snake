#ifndef BUTTONS_HPP
#define BUTTONS_HPP


#include <SDL3/SDL.h>
#include <string>
#include <stdexcept>
#include <memory>
#include "Logger.hpp"
#include "ColorConstants.hpp"
#include "Polygon.hpp"
#include "ScreenObject.hpp"


namespace snake {
class Buttons : protected ScreenObject {
private:

    void log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) const {
        Logger::log("Buttons::" + where, message, level);
    }
    template <typename ExceptionType>
    [[noreturn]] inline void logAndThrow(const std::string& where, const std::string& message) {
        Logger::logAndThrow<ExceptionType>("Buttons::" + where, message);
    }

public:
    explicit Buttons(SDL_Window* arg_window, SDL_Renderer* arg_renderer)
        try 
        : ScreenObject(arg_window, arg_renderer) {
            ScreenObject::is_clickable = true;
            ScreenObject::is_visible = true;
        } catch (std::exception& e) {
            logAndThrow<Logger::SeeAbove>(
                "Buttons constructor", 
                e.what()
            );
    }
};

}

#endif // BUTTONS_HPP