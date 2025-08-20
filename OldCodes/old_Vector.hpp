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

        // Constructor with std::array
        inline Vector(
            std::array<double, Dimension> arr
        ) : data(arr) {
        }

        inline Vector(const std::initializer_list<double>& il) {
            if (il.size() != Dimension) { 
                logAndThrow<std::invalid_argument>(
                    "Vector(const std::initializer_list& il)", 
                    "il should have length == Dimension"
                ); 
            }
            data = il;
        }

        // copy constructor
        inline Vector(const Vector&) = default;

        // move constructor
        inline Vector(Vector&&) = default;

        inline explicit Vector(
            const double& val = 0.0
        ) {
            data.fill(0.0);
        }

        inline explicit Vector(const double& x, const double& y) {
            static_assert(Dimension == 2, "This constructor is only usable when Dimension = 2");
            data = {x, y};
        }
        inline explicit Vector(const double& x, const double& y, const double& z) {
            static_assert(Dimension == 3, "This constructor is only usable when Dimension = 3");
            data = {x, y, z};
        }
        inline explicit Vector(const double& x, const double& y, const double& z, const double& w) {
            static_assert(Dimension == 4, "This constructor is only usable when Dimension = 4");
            data = {x, y, z, w};
        }

        // assignment operator
        inline Vector<Dimension>& operator=(Vector<Dimension> other) {
            if (this == &other) {
                return *this;
            }
            data = other.data;
            return *this;
        }


        inline const double& x() const {
            return data[0];
        }
        inline double& x() {
            return data[0];
        }

        inline const double& y() const {
            static_assert(Dimension > 1, "This function is only usable when Dimension >= 2");
            return data[1];
        }
        inline double& y() {
            static_assert(Dimension > 1, "This function is only usable when Dimension >= 2");
            return data[1];
        }

        inline const double& z() const {
            static_assert(Dimension > 2, "This function is only usable when Dimension >= 3");
            return data[2];
        }
        inline double& z() {
            static_assert(Dimension > 2, "This function is only usable when Dimension >= 3");
            return data[2];
        }

        inline const double& w() const {
            static_assert(Dimension > 3, "This function is only usable when Dimension >= 4");
            return data[3];
        }
        inline double& w() {
            static_assert(Dimension > 3, "This function is only usable when Dimension >= 4");
            return data[3];
        }

        inline double& operator[](const size_t& index) {
            if (index >= Dimension) {
                logAndThrow<std::out_of_range>(
                    "operator[](const size_t& index)", 
                    "index(" + std::to_string(index) + ") out of range [0,Dimension)"
                )
                throw;
            }
            return data[index];
        }  
        inline const double& operator[](const size_t& index) const {
            if (index >= Dimension) {
                logAndThrow<std::out_of_range>(
                    "operator[](const size_t& index)", 
                    "index(" + std::to_string(index) + ") out of range [0,Dimension)"
                )
                throw;
            }
            return data[index];
        }

        inline Vector<Dimension> operator-() const {
            Vector<Dimension> result;
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = -data[i];
            }
            return result;
        }
        inline Vector<Dimension> operator+(const Vector<Dimension>& other) const {
            Vector<Dimension> result;
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = data[i] + other[i];
            }
            return result;
        }
        inline Vector<Dimension> operator-(const Vector<Dimension>& other) const {
            Vector<Dimension> result;
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = data[i] - other[i];
            }
            return result;
        }
        inline Vector<Dimension> operator*(double scalar) const {
            Vector<Dimension> result;
            for (size_t i = 0; i < Dimension; ++i) {
                result[i] = data[i] * scalar;
            }
            return result;
        }
        inline Vector<Dimension> operator/(double scalar) const {
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

        inline Vector<Dimension>& operator+=(const Vector<Dimension>& other) {
            for (size_t i = 0; i < Dimension; ++i) {
                data[i] += other[i];
            }
            return *this;
        }
        inline Vector<Dimension>& operator-=(const Vector<Dimension>& other) {
            for (size_t i = 0; i < Dimension; ++i) {
                data[i] -= other[i];
            }
            return *this;
        }
        inline Vector<Dimension>& operator*=(double scalar) {
            for (size_t i = 0; i < Dimension; ++i) {
                data[i] *= scalar;
            }
            return *this;
        }
        inline Vector<Dimension>& operator/=(double scalar) {
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

        inline double magnitude() const {
            double sum = 0;
            for (size_t i = 0; i < Dimension; ++i) {
                sum += data[i] * data[i];
            }
            return std::sqrt(sum);
        }

        inline double dotMul(const Vector<Dimension>& other) const {
            double result = 0;
            for (size_t i = 0; i < Dimension; ++i) {
                result += data[i] * other[i];
            }
            return result;
        }
        inline double scalarCrossMul(const Vector<Dimension>& other) const {
            if constexpr (Dimension == 1) {
                return 0;
            } else if constexpr (Dimension == 2) {
                return data[0] * other[1] - other[0] * data[1];
            } else if constexpr (Dimension == 3) {
                return magnitude(cross_mul(other));
            }
            logAndThrow<std::runtime_error>("scalarCrossMul", "");
        }

        inline Vector<Dimension> crossMul(const Vector<Dimension>& other) const {
            static_assert(Dimension == 3, "crossMul can only be used for some dimensions of vectors");
        
        }

        inline bool isZeroVector(double precision = 1e-6) const {
            return is_equal(Vector<Dimension>::zero_vector(), precision);
        }
        inline bool isEqual(const Vector<Dimension>& other, double precision = 1e-6) const {
            for (size_t i = 0; i < Dimension; ++i)
                if (std::abs(data[i] - other[i]) > precision) {
                    return false;
                }
            return true;
        }
        inline bool isParallel(const Vector<Dimension>& other, double precision = 1e-6) const {
            double ratio = data[0] / other[0];
            for (size_t i = 1; i < Dimension; ++i) {
                if (!Utils::double_equal(ratio, data[i] / other[i])) {
                    return false;
                }
            }
            return true;
        }
        inline bool isPerpendicular(const Vector<Dimension>& other, double precision = 1e-6) const {
            return std::abs(dot_mul(other)) < precision;
        }


        inline static Vector<Dimension> zeroVector() {
            return Vector<Dimension>(0);
        }
    private:
        
        template <typename ExceptionType>
        [[noretuen]] inline void logAndThrow(std::string where, std::string message) {
            Logger::log_and_throw<ExceptionType>(
                "class Vector<" + std::to_string(Dimension) + ">::" + where, 
                message);
        }

}; // class Vector


} // namespace Math

#endif // VECTOR_HPP