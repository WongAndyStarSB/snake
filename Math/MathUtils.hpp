#ifndef MATH_HPP
#define MATH_HPP

#include <string>
#include <stdexcept>
#include <cmath>

namespace Math {
    const double PI = 3.14159265358979323846;

    class ZeroDivisionException : public std::exception {
    private:
        std::string default_msg = "divisor cannot be zero";
        std::string msg;
    
    public:
        inline explicit ZeroDivisionException(const char* arg_message = "")
            : msg((arg_message && *arg_message) ? arg_message : default_msg) 
        {}
        inline explicit ZeroDivisionException(const std::string& arg_message = "")
            : msg(arg_message.empty() ? default_msg : arg_message) 
        {}
        inline const char* what() const throw() override {
            return msg.c_str();
        }
    };
}

#endif // MATH_HPP