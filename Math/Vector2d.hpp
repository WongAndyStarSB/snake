#ifndef VECTOR_2D_HPP
#define VECTOR_2D_HPP

#include <string>
#include "../Logger.hpp"
#include "MathUtils.hpp"
#include "Vector.hpp"

namespace Math {

class Vector2d : protected Vector<2> {
    public:
        explicit Vector2d();
        explicit Vector2d(double arg_x, double arg_y);

        Vector2d(const Vector2d&);
        Vector2d(Vector2d&&);
        
        Vector2d(const std::array<double, 2>& arr);
        Vector2d(const std::initializer_list<double>& il);

        // Assignment operator
        Vector2d operator=(const Vector2d& other);
        Vector2d operator=(const Vector<2>& vect2d);

        const double& x() const { return data[0]; }
        const double& y() const { return data[1]; }

        std::array<double, 2> getArray() const;
        const std::array<double, 2>& getArrayConstReference() const;
        std::array<double, 2>& getArrayRefence();

        Vector<2> toBase() const;
        const Vector<2>* getBasePtr() const;
        const Vector<2>& getBaseConstReference() const;

        std::string toString(bool add_prefix = true) const;
        
        std::array<std::array<double, 1>, 2> toColumn() const;

        // Operators
        
        double& operator[](size_t index) { return Vector<2>::operator[](index); }
        const double& operator[](size_t index) const { return Vector<2>::operator[](index); }

        Vector2d operator-() const;

        Vector2d operator+(const Vector2d& other) const;
        Vector2d operator-(const Vector2d& other) const;
        Vector2d operator*(double scalar) const;
        Vector2d operator/(double scalar) const;
        
        Vector2d& operator+=(const Vector2d& other);
        Vector2d& operator-=(const Vector2d& other);
        Vector2d& operator*=(double scalar);
        Vector2d& operator/=(double scalar);
        
        double dotMul(const Vector2d& other) const;
        double scalarCrossMul(const Vector2d& other) const;
        double magnitude() const;

        bool isEqual(const Vector2d& other, double precision = 1e-6) const;
        bool isParallel(const Vector2d& other, double precision = 1e-6) const;
        bool isPerpendicular(const Vector2d& other, double precision = 1e-6) const;
        bool isZeroVector(double precision = 1e-6) const;
        
        static Vector2d zeroVector() { return Vector2d(0, 0); }
    private:
        template <typename ExceptionType>
        [[noreturn]] void logAndThrow(const std::string& func_name, const std::string& message) const;
};

}

#include "Vector2d.inl"

#endif // VECTOR_2D_HPP