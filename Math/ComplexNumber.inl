#ifndef COMPLEX_NUMBER_INL
#define COMPLEX_NUMBER_INL


#include "ComplexNumber.hpp"
#include "../Logger.hpp"

namespace Math {

template <typename ExceptionType>
[[noreturn]] void ComplexNumber::logAndThrow(const std::string& where, const std::string& what) const {
    Logger::logAndThrow("ComplexNumber::" + where, what);
}

}

#endif // COMPLEX_NUMBER_INL