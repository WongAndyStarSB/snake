#ifndef LOGGER_INL
#define LOGGER_INL


#include "Logger.hpp"
#include <type_traits>
#include "../Utils/StringUtils.hpp"



template <typename ExceptionType>
[[noreturn]] void Logger::logAndThrow(std::string_view where, std::string_view what) {
    static_assert(
        std::is_base_of<std::exception, ExceptionType>::value,
        "type must be derived from std::exception"
    );
    std::stringstream what_ss ("[ERROR] ");
    
    if constexpr (std::is_same<ExceptionType, Logger::SeeAbove>::value) {
        what_ss << "Logger::SeeAbove\n<SeeAbove> " << what;
    } else {
        what_ss << getCorrespondStrOfType<ExceptionType>() << "\n" << what;
    }
    Logger::logDos(where, std::string_view(what_ss.str()), Logger::LogLevel::ERROR, true);
    throw ExceptionType(std::string(what));
}

template <typename ExceptionType>
[[noreturn]] void Logger::log_and_throw(std::string_view where, std::string_view what) {
    logAndThrow(where, what);
}

// template <typename ExcpetionType>
// [[noreturn]] void Logger::logAndThrow(const std::stringstream& where, const std::stringstream& what) {
//     const std::string where_str = where.str();
//     const std::string what_str = what.str();
//     logAndThrow(std::string_view(where_str), std::string_view(what_str));
// }

template <typename ExceptionT>
void Logger::addTypeStringBond(const std::string& correspond_str) {
    s_typeid_to_str_map[typeid(ExceptionT)] = correspond_str;
}

template <typename ExceptionT>
std::string Logger::getCorrespondStrOfType() {
    std::type_index exceptionT_id (typeid(ExceptionT));
    if (s_typeid_to_str_map.find(exceptionT_id) == s_typeid_to_str_map.end()) {
        return "UnknownExceptionType";
    }
    return s_typeid_to_str_map[typeid(ExceptionT)];
}



#endif // LOGGER_INL