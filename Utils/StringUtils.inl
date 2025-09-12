#ifndef STRING_UTILS_INL
#define STRING_UTILS_INL

#include <string>
#include <sstream>
#include <type_traits>
#include <vector>
#include <functional>

#include "StringUtils.hpp"

namespace string_utils_ns {
    inline std::string add_indent(
        std::string_view txt, 
        const size_t& indent_num, 
        bool add_in_first_line_too,
        char indent_char
    ) {
        std::string indent_string (indent_num, indent_char);
        std::string result = 
            add_in_first_line_too 
            ? indent_string 
            : "";
        for (char c : txt) {
            result += c;
            if (c == '\n') {
                result += indent_string;
            }
        }
        return result;
    }

    inline std::stringstream& add_indent(
        std::stringstream& ss, 
        const size_t& indent_num, 
        const char& indent_char
    ) {
        std::string indent(indent_num, indent_char);
        std::string line;
        std::stringstream result;
        while (std::getline(ss, line)) {
            result << indent << line << '\n';
        }
        ss.str("");
        ss << result.str();
        return ss;
    }

    inline std::stringstream add_indent(
        const std::stringstream& ss, 
        const size_t& indent_num, 
        const char& indent_char
    ) {
        std::stringstream result_ss;
        result_ss << ss.rdbuf();
        add_indent(result_ss, indent_num, indent_char); // calling the one without const
        return result_ss;
    }

    template <typename ExceptionType>
    inline std::string common_exceptions_to_string() {
        static_assert(
            std::is_base_of<std::exception, ExceptionType>::value, 
            "type must derive from std::exception"
        );
        if constexpr (std::is_same<ExceptionType, std::runtime_error>::value) {
            return "std::runtime_error";
        } else if constexpr (std::is_same<ExceptionType, std::logic_error>::value) {
            return "std::logic_error";
        } else if constexpr (std::is_same<ExceptionType, std::domain_error>::value) {
            return "std::domain_error";
        } else if constexpr (std::is_same<ExceptionType, std::invalid_argument>::value) {
            return "std::invalid_argument";
        } else if constexpr (std::is_same<ExceptionType, std::out_of_range>::value) {
            return "std::out_of_range";
        } else if constexpr (std::is_same<ExceptionType, std::length_error>::value) {
            return "std::length_error";
        } else if constexpr (std::is_same<ExceptionType, std::runtime_error>::value) {
            return "std::runtime_error";
        } else if constexpr (std::is_same<ExceptionType, std::range_error>::value) {
            return "std::range_error";
        } else if constexpr (std::is_same<ExceptionType, std::overflow_error>::value) {
            return "std::overflow_error";
        } else if constexpr (std::is_same<ExceptionType, std::underflow_error>::value) {
            return "std::underflow_error";
        } else {
            return "unknown_exception";
        }
    }

    template <typename T>
    inline std::string to_string(const std::vector<T>& vect, bool with_prefix) {
        std::string return_str = ((with_prefix)? ("vector<"+get_class_name<T>()+">[") : ("[")); 
        if (vect.empty()) {
            return return_str+"-empty-]";
        }
        try {
            for (size_t i = 0; i < vect.size(); ++i) {
                return_str += vect[i].to_string(false) + ((i == vect.size()-1)? "]" : ", ");
            }
        } catch (const std::exception& e) {
            throw std::runtime_error(msg);
        }
        return return_str;
    }
    
    template <typename PtrT>
    inline std::string convertPtrToString(PtrT ptr) {
        std::stringstream ss;
        ss << static_cast<const void* const>(ptr);
        return ss.str();
    }

    template <typename IteratorT, typename ConverterT>
    std::string iterableToStr(IteratorT begin, IteratorT end, ConverterT converter) {
        if (begin == end) return "[ ]\n";
        std::ostringstream oss;
        oss << "[ ";
        for (IteratorT it = begin; it != end; ++it) {
            oss << converter(*it);
            if (std::next(it) != end) {
                oss << ", ";
            }
        }
        oss << " ]\n";
        return oss.str();
    }
}
    


#endif // STRING_UTILS_INL