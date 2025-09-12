#include "Logger.hpp"
#include "../Utils/StringUtils.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <string>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <mutex>
#include <cassert>


std::unordered_map<std::type_index, std::string> Logger::s_typeid_to_str_map = Logger::initTypeidToStrMap();

// Private helper: initialize s_typeid_to_str_map
std::unordered_map<std::type_index, std::string> Logger::initTypeidToStrMap() {
    std::unordered_map<std::type_index, std::string> m;
    m.reserve(16);

    m[typeid(std::runtime_error)] = "std::runtime_error";
    m[typeid(std::logic_error)] = "std::logic_error";
    m[typeid(std::domain_error)] = "std::domain_error";
    m[typeid(std::invalid_argument)] = "std::invalid_argument";
    m[typeid(std::out_of_range)] = "std::out_of_range";
    m[typeid(std::overflow_error)] = "std::overflow_error";
    m[typeid(std::underflow_error)] = "std::underflow_error";
    m[typeid(std::bad_alloc)] = "std::bad_alloc";
    m[typeid(std::bad_cast)] = "std::bad_cast";
    m[typeid(std::bad_typeid)] = "std::bad_typeid";
    m[typeid(std::exception)] = "std::exception";

    m[typeid(Logger::SeeAbove)] = "Logger::SeeAbove";

    return m;
}


void Logger::helperLogToFile(std::string_view file_name, std::string_view message) {

    std::lock_guard<std::mutex> lock(s_logtofile_mutex);
    static int num_of_same_name_files = 1;
    namespace fs = std::filesystem;
    fs::path log_path;
    log_path.concat("logs/").concat(file_name).concat(".log");
    
    try {
        if (fs::exists(log_path) && fs::file_size(log_path) > s_logfile_max_size) {
            log_path.replace_filename(log_path.stem().string() + "_" + std::to_string(++num_of_same_name_files));
        }
        fs::create_directories(log_path.parent_path());
        std::ofstream log_file(log_path, std::ios_base::app);
        if (log_file.is_open()) {
            log_file << message << std::endl;
        } else {
            std::cerr << "Unable to open log file: " << log_path << std::endl;
        }
        log_file.flush();
    } catch (const std::exception& e) {
        std::cerr << "Failed to log to file: " << e.what() << std::endl;
        throw e;
    }
}

std::string Logger::logLevelToString(LogLevel lev, bool add_sqbrackets) {
    if (add_sqbrackets) {
        return "[" + logLevelToString(lev, false) + "]";
    }
    switch (lev) {
        case INFO: return "INFO";
        case WARNING_LOW: return "WARNING_LOW";
        case WARNING_MID: return "WARNING_MID";
        case WARNING_HIGH: return "*WARNING_HIGH";
        case ERROR: return "***ERROR***";
        case DEBUG: return "debug";
        default: return "UNKNOWN";
    }
}

std::tm Logger::helperGetTime() {
    const int YEAR_OFFSET = 1900;
    std::time_t now = std::time(nullptr);
    std::tm local_time;
    #ifdef _MSC_VER
        localtime_s(&local_time, &now);
    #else
        local_time = *std::localtime(&now);
    #endif
    local_time.tm_year += YEAR_OFFSET;
    local_time.tm_mon += 1;
    return local_time;
}

std::string Logger::helperGenFileName(const std::tm& t) {
    std::string s;
    s.reserve(10);
    s.append(std::to_string(t.tm_year))
        .append("/").append(std::to_string(t.tm_mon))
        .append("-").append(std::to_string(t.tm_mday));
    return s;
}

std::string Logger::helperGenTimestamp(const std::tm& t) {
    std::string s;
    s.reserve(10);
    s.append("[")
        .append(std::to_string(t.tm_hour))
        .append(":").append(std::to_string(t.tm_min))
        .append(":").append(std::to_string(t.tm_sec))
        .append("]");
    return s;
}

void Logger::logDos(std::string_view where, std::string_view what, LogLevel lev, bool add_timestamp) {

    assert(!where.empty() && "where (std::string_view) cannot be empty/null");
    assert(!what.empty() && "what (std::string_view) cannot be empty/null");
    
    if (lev < log_level_threshold) {
        return;
    }

    std::tm local_time = helperGetTime();
    std::string log_entry;
    log_entry.reserve(256);
    log_entry
        .append((add_timestamp) ? helperGenTimestamp(local_time) : "[]")
        .append(" ")
        .append(logLevelToString(lev, true))
        .append("{\n") 
        .append(string_utils_ns::add_indent(where, s_num_of_indent_spaces))
        .append("\n")
        .append(string_utils_ns::add_indent(what, s_num_of_indent_spaces))
        .append("\n}");

    
    if (s_delay_log) {
        std::lock_guard<std::mutex> lock(s_logtobuffer_mutex);
        s_havent_logged_logs << "\n" << log_entry;
        if (lev == ERROR) {
            logHaventLogged();
        }
    } else {
        
        helperLogToFile(helperGenFileName(local_time), log_entry);
        
    }
    return;
}

// public

void Logger::log(std::string_view where, std::string_view what, Logger::LogLevel lev, bool add_timestamp) {
    logDos(where, what, lev, add_timestamp);
}
// void Logger::log(const std::stringstream& where, const std::stringstream& what, const LogLevel& lev, bool add_timestamp) {
//     const std::string where_str = where.str();  // Extend lifetime
//     const std::string what_str = what.str();
//     logDos(where_str, what_str, lev, add_timestamp);
// }

void Logger::logHaventLogged() {
    if (s_havent_logged_logs.str() != "") {
        std::string havent_logged_string = s_havent_logged_logs.str(); // extend lifetime
        helperLogToFile(
            helperGenFileName(helperGetTime()), 
            std::string_view(havent_logged_string)
        );
        s_havent_logged_logs.str("");
    }
}

void Logger::setDelayLog(bool new_delay_log) {
    if (new_delay_log == true) {
        s_delay_log = true;
        logHaventLogged();
    } else {
        s_delay_log = false;
    }
}

void Logger::start_run(const std::string& message) {
    log(
        "",
        std::string(20, '-')
            + " << program started to run >> "
            + std::string(50, '-')
            + ((message == "") ? ("") : ("\n" + string_utils_ns::add_indent(message, 2))),
        INFO,
        true
    );
}
