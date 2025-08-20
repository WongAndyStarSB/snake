#ifndef SCREEN_HPP
#define SCREEN_HPP


#include <string>
#include <stdexcept>
#include <memory>
#include <vector>

#include "Logger.hpp"

namespace snake {
class Screen {
private:
    // Helper function to log and throw exceptions
    template <typename ExceptionType>
    [[noreturn]] void log_and_throw(const std::string& where, const std::string& message) {
        Logger::log_and_throw<ExceptionType>("Screen::"+where, message);
    }
public:
    virtual ~Screen() = default;

    // Initializes the screen with optional actions
    // This function can be overridden by derived classes to provide specific initialization logic.
    virtual void init(std::vector<std::string> actions = {}) {
        if (actions.empty()) {
            log("init", "No actions provided for initialization", Logger::INFO);
        } else {
            log("init", "Actions provided: " + std::to_string(actions.size()), Logger::INFO);
            
            // left empty tmp
            log_and_throw<std::runtime_error>("init", "not finished");
        }
    }

    // Runs the main loop of the screen
    virtual void run() = 0;
    virtual void render() = 0;
};
}

#endif // SCREEN_HPP