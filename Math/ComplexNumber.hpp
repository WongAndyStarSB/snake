#ifndef COMPLEX_NUMBER_HPP
#define COMPLEX_NUMBER_HPP


#include "Vector2d.hpp"
#include <memory>
#include "Angle.hpp"

namespace Math {
class ComplexNumber : protected Vector2d {
    private:
        double precision = 1e-6;
        std::unique_ptr<double> Re;
        std::unique_ptr<double> Im;
    public:
        static ComplexNumber createFromPolar(double magnitude, const Angle& rad);
        explicit ComplexNumber();
        explicit ComplexNumber(double real);
        explicit ComplexNumber(double real, double imag);
        explicit ComplexNumber(const Vector2d& vect);
        explicit ComplexNumber(const std::initializer_list<double>& il);

        ComplexNumber(const ComplexNumber& other);
        ComplexNumber(ComplexNumber&& other);

        double getRe() const;
        double getIm() const;
        double getMagnitude() const;
        Angle getAngle() const;

        const double* const &cgetRePtr() const;
        const double* const &cgetImPtr() const;

        double* getRePtr();
        double* getImPtr();

        ComplexNumber operator+(double real) const;
        ComplexNumber operator-(double real) const;
        ComplexNumber operator*(double scalar) const;
        ComplexNumber operator/(double denominator) const;
        
        ComplexNumber operator+(const ComplexNumber& complex) const;
        ComplexNumber operator-(const ComplexNumber& complex) const;
        ComplexNumber operator*(const ComplexNumber& complex) const;
        ComplexNumber operator/(const ComplexNumber& complex) const;

        Vector2d toVector2d() const;
        ComplexNumber toConjugate() const;

        double mulWithComjugate() const;

        bool isReal() const;

        

    private:
        void helperInitReIm();

        void log(const std::string& where, const std::string& what, const Logger::LogLevel& lev) const;
        template <typename ExceptionType>
        [[noreturn]] void logAndThrow(const std::string& where, const std::string& what) const;

};
}

#include "Vector2d.inl"

#endif // COMPLEX_NUMBER_HPP