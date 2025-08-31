#ifndef MYEXCEPTION_HPP
#define MYEXCEPTION_HPP

#include <string>


class MyException: public std::exception {
    private:
        std::string message;
    public:
        MyException(const char* arg_message)
        : message(arg_message) {}
        MyException(const std::string arg_message)
        : message(arg_message) {}

        const char* what() const throw() {
            return message.c_str();
        }
};

#endif // MYEXCEPTION_HPP