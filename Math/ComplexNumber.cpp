#include "ComplexNumber.hpp"

#include <cstdlib>
#include <cmath>

namespace Math {

ComplexNumber ComplexNumber::createFromPolar(double magnitude, const Angle& rad) {
    return ComplexNumber(
        magnitude * std::cos(rad.getRadians()), 
        magnitude * std::sin(rad.getRadians())
    );
}

ComplexNumber::ComplexNumber(
) : Vector2d(0, 0) {
    helperInitReIm();
}

ComplexNumber::ComplexNumber(
    double real
) : Vector2d(real, 0) {
    helperInitReIm();
}

ComplexNumber::ComplexNumber(
    double real, double imag
) : Vector2d(real, imag) {
    helperInitReIm();
}

ComplexNumber::ComplexNumber(
    const Vector2d& vect
) : Vector2d(vect) {
    helperInitReIm();
}

ComplexNumber::ComplexNumber(
    const std::initializer_list<double>& il
) : Vector2d(il) {
    helperInitReIm();
}

ComplexNumber::ComplexNumber(const ComplexNumber& other) = default;

ComplexNumber::ComplexNumber(ComplexNumber&& other) = default;

// getters

double ComplexNumber::getRe() const {
    return (*Re);
}
double ComplexNumber::getIm() const {
    return (*Im);
}
double ComplexNumber::getMagnitude() const {
    return std::sqrt((*Re) * (*Re) + (*Im) * (*Im));
}
Angle ComplexNumber::getAngle() const {
    return Angle(std::atan2(*Im, *Re));
}

const double* const &ComplexNumber::cgetRePtr() const {
    return Re.get();
}
const double* const &ComplexNumber::cgetImPtr() const {
    return Im.get();
}

double* ComplexNumber::getRePtr() {
    return Re.get();
}
double* ComplexNumber::getImPtr() {
    return Im.get();
}

// operators with double

ComplexNumber ComplexNumber::operator+(double real) const {
    return ComplexNumber(
        (*Re) + real,
        (*Im)
    );
}

ComplexNumber ComplexNumber::operator-(double real) const {
    return ComplexNumber(
        (*Re) - real,
        (*Im)
    );
}

ComplexNumber ComplexNumber::operator*(double scalar) const {
    return ComplexNumber(
        (*Re) * scalar,
        (*Im) * scalar
    );
}

ComplexNumber ComplexNumber::operator/(double denominator) const {
    if (std::abs(denominator) < precision) {
        logAndThrow<ZeroDivisionException>(
            "ComplexNumber::operator/(double denominator) const",
            "denominator cannot be 0"
        );
    } 
    return ComplexNumber(
        (*Re) / denominator,
        (*Im) / denominator
    );
}

// operators with complex number

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const {
    return ComplexNumber(
        (*Re) + (*other.Re),
        (*Im) + (*other.Im)
    );
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) const {
    return ComplexNumber(
        (*Re) - (*other.Re),
        (*Im) - (*other.Im)
    );
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber& other) const {
    return ComplexNumber(
        (*Re) * (*other.Re) - (*Im) * (*other.Im),
        (*Re) * (*other.Im) + (*Im) * (*other.Re)
    );
} 

ComplexNumber ComplexNumber::operator/(const ComplexNumber& complex) const {
    double d = complex.mulWithComjugate();
    if (d < precision) { // d is in [0, inf)
        logAndThrow<ZeroDivisionException>(
            "operator/(const ComplexNumber& complex) const",
            "complex cannot be 0+0i"
        );
    }
    return this->operator*(complex.toConjugate()) / d;
}


Vector2d ComplexNumber::toVector2d() const {
    return Vector2d(*Re, *Im);
}

ComplexNumber ComplexNumber::toConjugate() const {
    return ComplexNumber((*Re), -(*Im));
}


double ComplexNumber::mulWithComjugate() const {
    return (*Re) * (*Re) + (*Im) * (*Im);
}

bool ComplexNumber::isReal() const {
    return std::abs(*Im) < precision;
}

void ComplexNumber::helperInitReIm() {
    this->Re = std::make_unique<double>(data[0]);
    this->Im = std::make_unique<double>(data[1]);
}

void ComplexNumber::log(const std::string &where, const std::string &what, const Logger::LogLevel& lev) const {
    Logger::log("ComplexNumber::" + where, what, lev);
}

}