#ifndef ANGLE_HPP
#define ANGLE_HPP


#include "../Logger/Logger.hpp"
#include "MathUtils.hpp"

namespace Math {

class Angle {

// member variables
private: 
    double radians;

// static
private:
    static Angle constructorNoCheck(double rad);
public:
    static double precision; 

// member functions
private:
    void helperNormalizeAngle();
    
    template <typename T>
    [[noreturn]] void log_and_throw(const std::string& where, const std::string& msg) const {
        Logger::log_and_throw<T>("Angle::" + where, msg);
    }

public:
    // default constructor
    explicit constexpr Angle() noexcept : radians(0.0) {}

    // constructor with double
    explicit Angle(double rad);

    // copy constructor
    Angle(const Angle& other) noexcept : radians(other.radians) {}
    // move constructor
    Angle(Angle&&) noexcept = default;

    double getRadians() const { return radians; }
    double getDegrees() const  { return radians * 180.0 / PI; }

    void setRadians(double rad);
    void setDegrees(double deg);
    
    Angle operator-() const;

    Angle operator+(const Angle& other) const;
    Angle operator-(const Angle& other) const;
    Angle& operator+=(const Angle& other);
    Angle& operator-=(const Angle& other);

    Angle operator+(double d) const;
    Angle operator-(double d) const;
    Angle& operator+=(double d);
    Angle& operator-=(double d);

    Angle operator*(double scalar) const;
    Angle operator/(double scalar) const;
    Angle& operator*=(double scalar);
    Angle& operator/=(double scalar);

    double operator*(const Angle& other) const;
    double operator/(const Angle& other) const;



}; // class Angle

}

#endif // ANGLE_HPP