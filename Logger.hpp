#ifndef LOGGER_HPP
#define LOGGER_HPP


#include <iostream>
#include <fstream>
#include <string>

#include "Utils.hpp"

enum LogLevel {
    INFO,
    WARNING_1,
    WARNING_2,
    WARNING_3,
    ERROR
};
class Logger {
  private:
    static void log_to_file(const std::string& message) {
        std::ofstream log_file("LOG.txt", std::ios_base::app);
        if (log_file.is_open()) {
            log_file << message << std::endl;
        } else {
            std::cerr << "Unable to open log file." << std::endl;
        }
        log_file.flush();
    }
    static std::string log_level_to_string(LogLevel lev) {
        switch (lev) {
            case INFO: return "INFO"; break;
            case WARNING_1: return "WARNING_1"; break;
            case WARNING_2: return "WARNING_2"; break;
            case WARNING_3: return "*WARNING_3"; break;
            case ERROR: return "***ERROR"; break;
            default: return "UNKNOWN";
        }
    }
  public:
    static void log(const std::string& message, LogLevel lev, bool add_timestamp = true) {
        std::string msg = Utils::String::add_indent(message, 4);
        if (add_timestamp) {
            std::time_t now = std::time(nullptr);
            std::tm local_time;
            #ifdef _MSC_VER
                localtime_s(&local_time, &now); // MSVC safe version
            #else
                local_time = *std::localtime(&now); // Standard C++ version (not thread-safe)
            #endif
            //'localtime': This function or variable may be unsafe. Consider using localtime_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details. [E:\coding\andy_coding_c\_snake\build\snake-Windows.vcxproj]MSVC(C4996)
            std::string timestamp = std::to_string(local_time.tm_year + 1900) + "-" +
                                    std::to_string(local_time.tm_mon + 1) + "-" +
                                    std::to_string(local_time.tm_mday) + " " +
                                    std::to_string(local_time.tm_hour) + ":" +
                                    std::to_string(local_time.tm_min) + ":" +
                                    std::to_string(local_time.tm_sec);
            log_to_file("[" + timestamp + "] [" + log_level_to_string(lev) + "] {\n" + msg + "\n}");
        } else {
            log_to_file("[] [" + log_level_to_string(lev) + "] {\n" + msg + "\n}");
        }
    }
    static void start_run(const std::string& message = "") {
        log(std::string(20, '-')+
        " << program started to run >> "+
        std::string(50, '-') + 
        ((message=="")? ("") : ("\n"+Utils::String::add_indent(message, 2))), INFO);
    }
    [[noreturn]] static void log_and_throw(const std::string& message) {
        log(message, ERROR);
        
        throw std::runtime_error(message);
    }
};

#endif // LOGGER_HPP
// End of file