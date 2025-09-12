#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <string>
#include <exception>
#include <utility>
#include <cmath>

#include "MathUtils.hpp"


namespace NS_math {

class Fraction {
    private:
        int numerator;
        int denominator;
    public:
        static int find_gcf(int n1, int n2);
        static long long find_gcf(long long n1, long long n2);
        static uint8_t default_precision;
        static Fraction get_fraction_from_double(double value, unsigned int precision = default_precision);

        explicit Fraction(const int& dividend = 0, const int& divisor = 1);
        explicit Fraction(double value, uint8_t precision = default_precision);
        inline Fraction(const Fraction& other) = default; // copy constructor
        inline Fraction(Fraction&&) noexcept = default; // move constructor

        void simplify();

        int get_numerator() const;
        int get_denominator() const;

        void set_numerator(int new_numerator);
        void set_denominator(int new_denominator);


        std::string to_string(bool add_prefix = true) const;
        double get_in_double() const;
        int floor() const;
        int ceil() const;
        std::pair<int, Fraction> get_mixed_fraction() const;


        
        // Assignment operator
        Fraction& operator=(const Fraction& other);
        Fraction& operator=(int num);
        Fraction& operator=(double value);

        bool operator==(const Fraction& other) const;
        bool operator<(const Fraction& other) const;
        bool operator>(const Fraction& other) const;
        bool operator!=(const Fraction& other) const;
        bool operator<=(const Fraction& other) const;
        bool operator>=(const Fraction& other) const;
        Fraction operator+(const Fraction& other) const;
        Fraction operator-(const Fraction& other) const;
        Fraction operator*(const Fraction& other) const;
        Fraction operator/(const Fraction& other) const;

        bool operator==(const int& num) const;
        bool operator<(const int& num) const;
        bool operator>(const int& num) const;
        bool operator!=(const int& num) const;
        bool operator<=(const int& num) const;
        bool operator>=(const int& num) const;
        Fraction operator+(const int& num) const;
        Fraction operator-(const int& num) const;
        Fraction operator*(const int& num) const;
        Fraction operator/(const int& num) const;
}; // class

} // namespace NS_math

#endif // FRACTION_HPP