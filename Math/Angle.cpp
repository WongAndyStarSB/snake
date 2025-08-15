#include "Angle.hpp"
#include "MathUtils.hpp"


#include <cmath>
#include <cstdlib>


using namespace Math;


// public static

double Angle::precision = 1e-6;


// public constructors

Angle::Angle(double rad) 
    : radians(rad) { 
    if (!std::isfinite(rad)) {
        log_and_throw<std::invalid_argument>(
            "Angle(double rad)", 
            "Invalid angle: rad must be a finite value."
        );
    }
    helperNormalizeAngle(); 
}

void Angle::setRadians(double rad) { 
    if (!std::isfinite(rad)) {
        log_and_throw<std::invalid_argument>(
            "Angle::setRadians(double rad)", 
            "Invalid angle: rad must be a finite value."
        );
    }
    radians = rad;
    helperNormalizeAngle();
}
void Angle::setDegrees(double deg) { 
    if (!std::isfinite(deg)) {
        log_and_throw<std::invalid_argument>(
            "Angle::setDegrees(double deg)", 
            "Invalid angle: deg must be a finite value."
        );
    }
    radians = deg * PI / 180.0;
    helperNormalizeAngle();
}

Angle Angle::operator-() const {
    return constructorNoCheck(PI*2 - radians);
}

Angle Angle::operator+(const Angle& other) const {
    return Angle(radians + other.radians);
}
Angle Angle::operator-(const Angle& other) const {
    return Angle(radians - other.radians);
}
Angle& Angle::operator+=(const Angle& other) {
    radians += other.radians;
    helperNormalizeAngle();
    return *this;
}
Angle& Angle::operator-=(const Angle& other) {
    radians -= other.radians;
    helperNormalizeAngle();
    return *this;
}

Angle Angle::operator+(double d) const {
    if (!std::isfinite(d)) {
        log_and_throw<std::invalid_argument>(
            "operator+(double d) const", 
            "Invalid angle: d must be a finite value."
        );
    }
    return Angle(radians + d);
}
Angle Angle::operator-(double d) const {
    if (!std::isfinite(d)) {
        log_and_throw<std::invalid_argument>(
            "operator-(double d) const", 
            "Invalid angle: d must be a finite value."
        );
    }
    return Angle(radians - d);
}
Angle& Angle::operator+=(double d) {
    if (!std::isfinite(d)) {
        log_and_throw<std::invalid_argument>(
            "operator+=(double d)", 
            "Invalid angle: d must be a finite value."
        );
    }
    radians += d;
    helperNormalizeAngle();
    return *this;
}
Angle& Angle::operator-=(double d) {
    if (!std::isfinite(d)) {
        log_and_throw<std::invalid_argument>(
            "operator-=(double d)", 
            "Invalid angle: d must be a finite value."
        );
    }
    radians -= d;
    helperNormalizeAngle();
    return *this;
}

Angle Angle::operator*(double scalar) const {
    if (!std::isfinite(scalar)) {
        log_and_throw<std::invalid_argument>(
            "operator*(double scalar) const", 
            "Invalid angle: scalar must be a finite value."
        );
    }
    return Angle(radians * scalar);
}
Angle Angle::operator/(double scalar) const {
    if (!std::isfinite(scalar)) {
        log_and_throw<std::invalid_argument>(
            "operator/(double scalar) const", 
            "Invalid angle: scalar must be a finite value."
        );
    }
    if (std::abs(scalar) < precision) {
        log_and_throw<ZeroDivisionException>(
            "operator/(double scalar) const",
            "scalar cannot be zero"
        );
    }
    return Angle(radians / scalar);
}
Angle& Angle::operator*=(double scalar) {
    if (!std::isfinite(scalar)) {
        log_and_throw<std::invalid_argument>(
            "operator*=(double scalar)", 
            "Invalid angle: scalar must be a finite value."
        );
    }
    radians *= scalar;
    return *this;
}
Angle& Angle::operator/=(double scalar) {
    if (!std::isfinite(scalar)) {
        log_and_throw<std::invalid_argument>(
            "operator/=(double scalar)", 
            "Invalid angle: scalar must be a finite value."
        );
    }
    if (std::abs(scalar) < precision) {
        log_and_throw<ZeroDivisionException>(
            "operator/=(double scalar)",
            "scalar cannot be zero"
        );
    }
    radians /= scalar;
    return *this;
}

double Angle::operator*(const Angle& other) const {
    return radians * other.radians;
}
double Angle::operator/(const Angle& other) const {
    if (std::abs(other.radians) < precision) {
        log_and_throw<ZeroDivisionException>(
            "operator/(const Angle& other) const",
            "scalar cannot be zero"
        );
    }
    return radians / other.radians;
}

// private

Angle Angle::constructorNoCheck(double rad) {
    Angle a;
    a.radians = rad;
    return a;
}

void Angle::helperNormalizeAngle() {
    // radians %= PI;
    radians = fmod(radians, PI * 2);
    radians = (radians < 0 ? radians + PI * 2 : radians);
}