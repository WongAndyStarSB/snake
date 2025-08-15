#ifndef VECTOR_INL
#define VECTOR_INL

#include "Vector.hpp"

namespace Math {

template <size_t Dimension>
Vector<Dimension>::Vector(std::array<double, Dimension> arr) : data(arr) {}

template <size_t Dimension>
Vector<Dimension>::Vector(const std::initializer_list<double>& il) {
    if (il.size() != Dimension) { 
        logAndThrow<std::invalid_argument>(
            "Vector(const std::initializer_list& il)", 
            "il should have length == Dimension"
        ); 
    }
    data = il;
}

template <size_t Dimension>
Vector<Dimension>::Vector(const Vector&) = default;

template <size_t Dimension>
Vector<Dimension>::Vector(Vector&&) = default;

template <size_t Dimension>
Vector<Dimension>::Vector(const double& val) {
    data.fill(0.0);
}

// template <size_t Dimension>
// Vector<Dimension>::Vector(const double& x, const double& y) {
//     static_assert(Dimension == 2, "This constructor is only usable when Dimension = 2");
//     data = {x, y};
// }

// template <size_t Dimension>
// Vector<Dimension>::Vector(const double& x, const double& y, const double& z) {
//     static_assert(Dimension == 3, "This constructor is only usable when Dimension = 3");
//     data = {x, y, z};
// }

// template <size_t Dimension>
// Vector<Dimension>::Vector(const double& x, const double& y, const double& z, const double& w) {
//     static_assert(Dimension == 4, "This constructor is only usable when Dimension = 4");
//     data = {x, y, z, w};
// }

template <size_t Dimension>
Vector<Dimension>& Vector<Dimension>::operator=(Vector<Dimension> other) {
    if (this == &other) {
        return *this;
    }
    data = other.data;
    return *this;
}

template <size_t Dimension>
std::array<double, Dimension> Vector<Dimension>::getData() const {
    return data;
}
template <size_t Dimension>
std::array<double, Dimension>& Vector<Dimension>::getDataReference() {
    return data;
}
template <size_t Dimension>
const std::array<double, Dimension>& Vector<Dimension>::getDataConstReference() const {
    return data;
}

template <size_t Dimension>
double& Vector<Dimension>::operator[](const size_t& index) {
    if (index >= Dimension) {
        logAndThrow<std::out_of_range>(
            "operator[](const size_t& index)", 
            "index(" + std::to_string(index) + ") out of range [0,Dimension)"
        );
    }
    return data[index];
}

template <size_t Dimension>
const double& Vector<Dimension>::operator[](const size_t& index) const {
    if (index >= Dimension) {
        logAndThrow<std::out_of_range>(
            "operator[](const size_t& index)", 
            "index(" + std::to_string(index) + ") out of range [0,Dimension)"
        );
    }
    return data[index];
}

template <size_t Dimension>
Vector<Dimension> Vector<Dimension>::operator-() const {
    Vector<Dimension> result;
    for (size_t i = 0; i < Dimension; ++i) {
        result[i] = -data[i];
    }
    return result;
}

template <size_t Dimension>
Vector<Dimension> Vector<Dimension>::operator+(const Vector<Dimension>& other) const {
    Vector<Dimension> result;
    for (size_t i = 0; i < Dimension; ++i) {
        result[i] = data[i] + other[i];
    }
    return result;
}

template <size_t Dimension>
Vector<Dimension> Vector<Dimension>::operator-(const Vector<Dimension>& other) const {
    Vector<Dimension> result;
    for (size_t i = 0; i < Dimension; ++i) {
        result[i] = data[i] - other[i];
    }
    return result;
}

template <size_t Dimension>
Vector<Dimension> Vector<Dimension>::operator*(double scalar) const {
    Vector<Dimension> result;
    for (size_t i = 0; i < Dimension; ++i) {
        result[i] = data[i] * scalar;
    }
    return result;
}

template <size_t Dimension>
Vector<Dimension> Vector<Dimension>::operator/(double scalar) const {
    if (scalar == 0) { 
        logAndThrow<ZeroDivisionError>(
            "operator/(double scalar) const", 
            "dividing by scalar, scalar cannot be zero"
        );
    }
    Vector<Dimension> result;
    for (size_t i = 0; i < Dimension; ++i) {
        result[i] = data[i] / scalar;
    }
    return result;
}

template <size_t Dimension>
Vector<Dimension>& Vector<Dimension>::operator+=(const Vector<Dimension>& other) {
    for (size_t i = 0; i < Dimension; ++i) {
        data[i] += other[i];
    }
    return *this;
}

template <size_t Dimension>
Vector<Dimension>& Vector<Dimension>::operator-=(const Vector<Dimension>& other) {
    for (size_t i = 0; i < Dimension; ++i) {
        data[i] -= other[i];
    }
    return *this;
}

template <size_t Dimension>
Vector<Dimension>& Vector<Dimension>::operator*=(double scalar) {
    for (size_t i = 0; i < Dimension; ++i) {
        data[i] *= scalar;
    }
    return *this;
}

template <size_t Dimension>
Vector<Dimension>& Vector<Dimension>::operator/=(double scalar) {
    if (scalar == 0) { 
        logAndThrow<ZeroDivisionError>(
            "operator/(double scalar) const", 
            "dividing by scalar, scalar cannot be zero"
        );
    }
    for (size_t i = 0; i < Dimension; ++i) {
        data[i] /= scalar;
    }
    return *this;
}

template <size_t Dimension>
double Vector<Dimension>::magnitude() const {
    double sum = 0;
    for (size_t i = 0; i < Dimension; ++i) {
        sum += data[i] * data[i];
    }
    return std::sqrt(sum);
}

template <size_t Dimension>
double Vector<Dimension>::dotMul(const Vector<Dimension>& other) const {
    double result = 0;
    for (size_t i = 0; i < Dimension; ++i) {
        result += data[i] * other[i];
    }
    return result;
}

template <size_t Dimension>
double Vector<Dimension>::scalarCrossMul(const Vector<Dimension>& other) const {
    if constexpr (Dimension == 1) {
        return 0;
    } else if constexpr (Dimension == 2) {
        return data[0] * other[1] - other[0] * data[1];
    } else if constexpr (Dimension == 3) {
        return magnitude(cross_mul(other));
    }
    logAndThrow<std::runtime_error>("scalarCrossMul", "");
}

template <size_t Dimension>
Vector<Dimension> Vector<Dimension>::crossMul(const Vector<Dimension>& other) const {
    static_assert(Dimension == 3, "crossMul can only be used for some dimensions of vectors");
}

template <size_t Dimension>
bool Vector<Dimension>::isZeroVector(double precision) const {
    return is_equal(Vector<Dimension>::zero_vector(), precision);
}

template <size_t Dimension>
bool Vector<Dimension>::isEqual(const Vector<Dimension>& other, double precision) const {
    for (size_t i = 0; i < Dimension; ++i)
        if (std::abs(data[i] - other[i]) > precision) {
            return false;
        }
    return true;
}

template <size_t Dimension>
bool Vector<Dimension>::isParallel(const Vector<Dimension>& other, double precision) const {
    double ratio = data[0] / other[0];
    for (size_t i = 1; i < Dimension; ++i) {
        if (!Utils::double_equal(ratio, data[i] / other[i])) {
            return false;
        }
    }
    return true;
}

template <size_t Dimension>
bool Vector<Dimension>::isPerpendicular(const Vector<Dimension>& other, double precision) const {
    return std::abs(dot_mul(other)) < precision;
}

template <size_t Dimension>
Vector<Dimension> Vector<Dimension>::zeroVector() {
    return Vector<Dimension>(0);
}

template <size_t Dimension>
template <typename ExceptionType>
void Vector<Dimension>::logAndThrow(std::string where, std::string message) {
    Logger::log_and_throw<ExceptionType>(
        "class Vector<" + std::to_string(Dimension) + ">::" + where, 
        message);
}

} // namespace Math

#endif // VECTOR_INL