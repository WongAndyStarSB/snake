#include "Vector2d.hpp"

#include "ZeroDivisionException.hpp"

namespace NS_math {

Vector2d::Vector2d() 
    : Vector<2>(0) 
{}

Vector2d::Vector2d(double arg_x, double arg_y)
    : Vector<2>{ arg_x, arg_y }
{}

Vector2d::Vector2d(const Vector2d&) = default;
Vector2d::Vector2d(Vector2d&&) = default;

Vector2d::Vector2d(const std::array<double, 2>& arr)
    : Vector<2>(arr) 
{}

Vector2d::Vector2d(const std::initializer_list<double>& il)
    : Vector<2>(0) {
    if (il.size() != 2) { 
        logAndThrow<std::invalid_argument>(
            "Vector2d::Vector2d(const std::initializer_list& il)", 
            "il should have length = 2"
        ); 
    }
    const double* ptr = il.begin();
    data[0] = *ptr;
    data[1] = *++ptr;
}

Vector2d Vector2d::operator=(const Vector2d& other) {
    if (this == &other) {
        return *this; // self-assignment check
    }
    data = other.data;
    return *this;
}

Vector2d Vector2d::operator=(const Vector<2>& vect2d) {
    data = vect2d.getData();
    return *this;
}

std::array<double, 2> Vector2d::getArray() const {
    return data;
}
const std::array<double, 2>& Vector2d::getArrayConstReference() const {
    return data;
}
std::array<double, 2>& Vector2d::getArrayRefence() {
    return data;
}

Vector<2> Vector2d::toBase() const {
    return *this;
}
const Vector<2>* Vector2d::getBasePtr() const {
    return this;
}
const Vector<2>& Vector2d::getBaseConstReference() const {
    return *this;
}

std::string Vector2d::toString(bool add_prefix) const {
    return 
        (add_prefix? "Vector2d( " : "( ") 
        + std::to_string(data[0]) + "i + " 
        + std::to_string(data[1]) + "j )";
}

std::array<std::array<double, 1>, 2> Vector2d::toColumn() const {
    std::array<std::array<double, 1>, 2> column;
    column[0][0] = data[0];
    column[1][0] = data[1];
    return column;
}

Vector2d Vector2d::operator-() const {
    return Vector2d(-data[0], -data[1]);
}

Vector2d Vector2d::operator+(const Vector2d& other) const {
    return Vector2d(data[0] + other[0], data[1] + other[1]);
}

Vector2d Vector2d::operator-(const Vector2d& other) const {
    return Vector2d(data[0] - other[0], data[1] - other[1]);
}

Vector2d Vector2d::operator*(double scalar) const {
    return Vector2d(data[0] * scalar, data[1] * scalar);
}

Vector2d Vector2d::operator/(double scalar) const {
    if (scalar == 0) { 
        logAndThrow<ZeroDivisionException>(
            "operator/(double scalar) const", 
            "dividing by scalar, scalar cannot be zero"
        );
    }
    return Vector2d(data[0] * scalar, data[1] * scalar);
}

Vector2d& Vector2d::operator+=(const Vector2d& other) {
    data[0] += other[0];
    data[1] += other[1];
    return *this;
}

Vector2d& Vector2d::operator-=(const Vector2d& other) {
    data[0] -= other[0];
    data[1] -= other[1];
    return *this;
}

Vector2d& Vector2d::operator*=(double scalar) {
    data[0] *= scalar;
    data[1] *= scalar;
    return *this;
}

Vector2d& Vector2d::operator/=(double scalar) {
    if (scalar == 0) { 
        logAndThrow<ZeroDivisionException>(
            "operator/(double scalar) const", 
            "dividing by scalar, scalar cannot be zero"
        );
    }
    data[0] /= scalar;
    data[1] /= scalar;
    return *this;
}

double Vector2d::dotMul(const Vector2d& other) const {
    return data[0] * other[0] + data[1] * other[1];
}

double Vector2d::scalarCrossMul(const Vector2d& other) const {
    return data[0] * other[1] - other[0] * data[1];
}

double Vector2d::magnitude() const {
    return std::sqrt(data[0] * data[0] + data[1] * data[1]);
}

bool Vector2d::isEqual(const Vector2d& other, double precision) const {
    return 
        std::abs(data[0] - other[0]) < precision 
        && std::abs(data[1] - other[1]) < precision;
}

bool Vector2d::isParallel(const Vector2d& other, double precision) const {
    return std::abs(scalarCrossMul(other)) < precision;
}

bool Vector2d::isPerpendicular(const Vector2d& other, double precision) const {
    return std::abs(dotMul(other)) < precision;
}

bool Vector2d::isZeroVector(double precision) const {
    return isEqual(Vector2d::s_zeroVector(), precision);
}



std::string Vector2d::s_toString(const Vector2d& vect) {
    return vect.toString(false);
}

}