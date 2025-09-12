#ifndef MATH_ZERO_DIVISION_EXCEPTION
#define MATH_ZERO_DIVISION_EXCEPTION


#include <stdexcept>

#include <string_view>

namespace NS_math {
    class ZeroDivisionException : public std::exception {
    private:
        static inline const char s_default_msg[] = "division by zero is undefined";
    private:
        std::string m_msg;
    private:
        // nested struct for class initialization
        struct ClassInitializer {
            explicit ClassInitializer();
        };
        // for calling the class initialization function (constructor of ClassInitializer)
        static ClassInitializer s_initializer;
    public:
        explicit ZeroDivisionException(const char* message = "");

        explicit ZeroDivisionException(std::string_view message);

        const char* what() const throw() override;
    };
}

#endif // MATH_ZERO_DIVISION_EXCEPTION