#ifndef VECTOR_3d_HPP
#define VECTOR_3d_HPP

#include <string>
#include <array>
#include <type_traits>
#include <stdexcept>
#include <iterator>

#include "Logger.hpp"

class Vector3d {
    public:
        double x;
        double y;
        double z;
        inline explicit Vector3d()
            : x(0.0), y(0.0), z(0.0) 
        {}
        inline explicit Vector3d(double arg_x, double arg_y, double arg_z) 
            : x(arg_x), y(arg_y), z(arg_z) 
        {}
        inline Vector3d(const Vector3d&) = default;
        inline Vector3d(Vector3d&&) = default;

        // std::enable_if<bool>: Enables/disables the constructor based on a condition
        // std::dacay<type>: Removes references and cv-qualifiers from a type
        // std::is_convertible<obj, type>: Checks if one type can be converted to another
        // std::declval<type>(): Pretends to create a value of a type for type deduction
        // SFINAE
        
        // constructor that can accept all type of containers as argument
        // only used when (Container != Vector3d) && ((*(Container_obj.begin())) is_convertible to bool)
        template <
            typename Container, 
            typename = typename std::enable_if<
                !std::is_same<typename std::decay<Container>::type, Vector3d>::value &&
                std::is_convertible<decltype(*std::begin(std::declval<Container>())), double>::value
            >::type
        >
        Vector3d(const Container& container) {
            auto it = std::begin(container);
            if (std::distance(it, std::end(container)) != 3) { 
                Logger::log_and_throw<std::invalid_argument>(
                    "template <typename Container> Vector3d::Vector3d(const Container& container)", 
                    "container should have length = 3"); 
                }
            x = *it++;
            y = *it++;
            z = *it;
        }
        

        

        // Assignment operator
        inline Vector3d operator=(const Vector3d& other) {
            if (this == &other) {
                return *this; // self-assignment check
            }
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        std::string to_string(bool add_prefix = true) const;
        std::array<double, 3> to_array() const;
        std::array<std::array<double, 1>, 3> to_column() const;

        // Operators

        double& operator[](size_t index);
        const double& operator[](size_t index) const;

        Vector3d operator+(const Vector3d& other) const;
        Vector3d operator-(const Vector3d& other) const;
        Vector3d operator*(double scalar) const;
        Vector3d operator/(double scalar) const;
        double dot_mul(const Vector3d& other) const;
        Vector3d cross_mul(const Vector3d& other) const;
        double magnitude() const;
        Vector3d unit_vector() const;

        bool is_zero_vector(double precision = 1e-6) const;
        bool is_equal(const Vector3d& other, double precision = 1e-6) const;
        bool is_parallel(const Vector3d& other, double precision = 1e-6) const;
        bool is_perpendicular(const Vector3d& other, double precision = 1e-6) const;

        static Vector3d zero_vector();

        

    private:
        template <typename ExceptionType>
        [[noreturn]] inline void log_and_throw(const std::string& func_name, const std::string& message) const {
            Logger::log_and_throw<ExceptionType>(
                "Vector3d::" + func_name, 
                message);
        }
};




#endif