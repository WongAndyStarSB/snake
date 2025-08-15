#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include "Logger.hpp"
#include "Utils.hpp"

namespace Math {

template <size_t Dimension>
class Vector {
    static_assert(Dimension > 0, "Dimension should be larger than 0");

protected:
    std::array<double, Dimension> data;

public:
    Vector(std::array<double, Dimension> arr);
    Vector(const std::initializer_list<double>& il);
    Vector(const Vector&) = default;
    Vector(Vector&&) = default;
    explicit Vector(const double& val = 0.0);
    // explicit Vector(const double& x, const double& y);
    // explicit Vector(const double& x, const double& y, const double& z);
    // explicit Vector(const double& x, const double& y, const double& z, const double& w);

    Vector<Dimension>& operator=(Vector<Dimension> other);

    const double& x() const;
    double& x();
    const double& y() const;
    double& y();
    const double& z() const;
    double& z();
    const double& w() const;
    double& w();

    std::array<double, Dimension> getData() const;
    std::array<double, Dimension>& getDataReference();
    const std::array<double, Dimension>& getDataConstReference() const;

    double& operator[](const size_t& index);
    const double& operator[](const size_t& index) const;

    Vector<Dimension> operator-() const;
    Vector<Dimension> operator+(const Vector<Dimension>& other) const;
    Vector<Dimension> operator-(const Vector<Dimension>& other) const;
    Vector<Dimension> operator*(double scalar) const;
    Vector<Dimension> operator/(double scalar) const;

    Vector<Dimension>& operator+=(const Vector<Dimension>& other);
    Vector<Dimension>& operator-=(const Vector<Dimension>& other);
    Vector<Dimension>& operator*=(double scalar);
    Vector<Dimension>& operator/=(double scalar);

    double magnitude() const;
    double dotMul(const Vector<Dimension>& other) const;
    double scalarCrossMul(const Vector<Dimension>& other) const;
    Vector<Dimension> crossMul(const Vector<Dimension>& other) const;

    bool isZeroVector(double precision = 1e-6) const;
    bool isEqual(const Vector<Dimension>& other, double precision = 1e-6) const;
    bool isParallel(const Vector<Dimension>& other, double precision = 1e-6) const;
    bool isPerpendicular(const Vector<Dimension>& other, double precision = 1e-6) const;

    static Vector<Dimension> zeroVector();

private:
    template <typename ExceptionType>
    void logAndThrow(std::string where, std::string message);
};

} // namespace Math

#include "Vector.inl"

#endif // VECTOR_HPP