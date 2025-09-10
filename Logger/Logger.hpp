#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <mutex>



class Logger {
public:
    enum LogLevel {
        DEBUG,
        INFO,
        WARNING_LOW,
        WARNING_MID,
        WARNING_HIGH,
        ERROR
    };
    
    class SeeAbove : public std::exception {
        std::string msg_;
        public:
            inline explicit SeeAbove(
                const std::string& message
            ) : msg_("<SeeAbove> " + message) {}

            inline const char* what() const noexcept override {
                return msg_.c_str();
            }
    };

private:
    static std::mutex s_logtofile_mutex;
    static std::mutex s_logtobuffer_mutex;

    static std::stringstream s_havent_logged_logs;
    // Map from type_info to string for type names 
    static std::unordered_map<std::type_index, std::string> s_typeid_to_str_map;

    static std::unordered_map<std::type_index, std::string> initTypeidToStrMap();

    // Private helper: log to file
    // thread safty held by using lock_guard
    static void helperLogToFile(std::string_view file_name, std::string_view message);
    // Private helper: log level to string
    // Log levels are ordered from least severe (DEBUG) to most severe (ERROR).
    static std::string logLevelToString(LogLevel lev, bool add_sqbrackets = true);
    
    static void logDos(std::string_view where, std::string_view what, LogLevel lev, bool add_timestamp);

    static std::tm helperGetTime();
    static std::string helperGenFileName(const std::tm& t);
    static std::string helperGenTimestamp(const std::tm& t);

public:


    static LogLevel log_level_threshold;
    static bool s_delay_log;
    static size_t s_num_of_indent_spaces;
    static double s_logfile_max_size;

    // static void log(const std::string& where, const std::string& what, const LogLevel& lev, bool add_timestamp = true);
    
    // static void log(const std::stringstream& where, const std::stringstream& what, const LogLevel& lev, bool add_timestamp = true);
    
    static void log(std::string_view where, std::string_view what, LogLevel lev, bool add_timestamp = true);

    static void start_run(const std::string& message = "");

    template <typename ExceptionType = std::runtime_error>
    static [[noreturn]] void log_and_throw(std::string_view where, std::string_view what);

    template <typename ExceptionType = std::runtime_error>
    static [[noreturn]] void logAndThrow(std::string_view where, std::string_view what);

    // template <typename ExceptionType = std::runtime_error>
    // static [[noreturn]] void logAndThrow(const std::stringstream& where, const std::stringstream& what);

    static void logHaventLogged();

    template <typename ExceptionT>
    static void addTypeStringBond(const std::string& correspond_str);

    template <typename ExceptionT>
    static const std::string& getCorrespondStrOfType();

}; // class Logger

#include "Logger.inl" // For template implementation

#endif // LOGGER_HPP