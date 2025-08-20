#ifndef MAIN_SCREEN_HPP
#define MAIN_SCREEN_HPP

#include <string>
#include <stdexcept>
#include <memory>
#include "Screen.hpp"
#include "Logger.hpp"

namespace snake {

class MainScreen : public Screen {
private:

    void log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) const {
        Logger::log("MainScreen::" + where, message, level);
    }

    template <typename ExceptionType>
    [[noreturn]] inline void log_and_throw(const std::string& where, const std::string& message) {
        Logger::log_and_throw<ExceptionType>("MainScreen::" + where, message);
    }


};

}



#endif // MAIN_SCREEN_HPP