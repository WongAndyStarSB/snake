#ifndef VECTOR_2D_INL
#define VECTOR_2D_INL

#include "Vector2d.hpp"

namespace Math {

template <typename ExceptionType>
inline void Vector2d::logAndThrow(const std::string& func_name, const std::string& message) const {
    Logger::logAndThrow<ExceptionType>(
        "Vector2d::" + func_name, 
        message);
}

}

#endif // VECTOR_2D_INL