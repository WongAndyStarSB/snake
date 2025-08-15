#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP


#include <array>
#include <vector>
#include <string>

class Vector2D {
    public:
        int x;
        int y;
        inline explicit Vector2D(int arg_x, int arg_y) noexcept : x(arg_x), y(arg_y) {}
        inline explicit Vector2D() noexcept : x(0), y(0) {}
        // copy constructor
        inline Vector2D(const Vector2D& other) noexcept : x(other.x), y(other.y) {}
        // move constructor
        inline Vector2D(Vector2D&& other) noexcept : x(other.x), y(other.y) {
            other.x = 0;
            other.y = 0;
        }
        // array constructor
        inline Vector2D(int arr[2]) noexcept : x(arr[0]), y(arr[1]) {}
        // array constructor with std::array
        inline Vector2D(const std::array<int, 2>& arr) noexcept : x(arr[0]), y(arr[1]) {}
        
        // assignment operator
        Vector2D& operator=(const Vector2D& other) noexcept = default;
        // move assignment operator
        Vector2D& operator=(Vector2D&& other) noexcept = default;

        bool isVectorInRange(int upper_lim_x, int upper_lim_y, int lower_lim_x = 0, int lower_lim_y = 0);
        bool isVectorInRange(Vector2D upper_lim, Vector2D lower_lim = Vector2D(0, 0));

        std::string to_string(bool with_prefix = true) const ;
        std::array<int, 2> to_array() const ;
        std::array<double, 2> to_array_double() const;

        static std::string vector_to_string(const std::vector<Vector2D>& vect, bool with_prefix = true);

        static Vector2D get_zero_vector() noexcept;
        static Vector2D get_up_vector() noexcept;
        static Vector2D get_down_vector() noexcept;
        static Vector2D get_left_vector() noexcept;
        static Vector2D get_right_vector() noexcept;

        bool is_same_direction_with(const Vector2D& other, bool zero_vector_consider_as_true) const noexcept;
        bool is_opposite_direction_with(const Vector2D& other, bool zero_vector_consider_as_true) const noexcept;
        bool is_parallel_with(const Vector2D& other, bool zero_vector_consider_as_true = true) const noexcept;

        void change_value_as(int new_x, int new_y);
        void change_value_as(Vector2D other);

        Vector2D operator+(const Vector2D& other) const noexcept;
        Vector2D operator-(const Vector2D& other) const noexcept;
        Vector2D operator*(const int& scalar) const noexcept;
        Vector2D operator*(const double& scalar) const noexcept;
        
        Vector2D& operator+=(const Vector2D& other) noexcept;
        Vector2D& operator-=(const Vector2D& other) noexcept;
        Vector2D& operator*=(const int& scalar) noexcept;
        Vector2D& operator*=(const double& scalar) noexcept;

        int dot_mul(const Vector2D& other) const noexcept;
        // determinant of the matrix formed by this vector and the other vector
        // returns the area of the parallelogram formed by the two vectors
        int det(const Vector2D& other) const noexcept;
        // scalar cross product
        // returns the area of the parallelogram formed by the two vectors
        int scalar_cross_mul(const Vector2D& other) const noexcept { return this->det(other); }

        
        bool operator==(const Vector2D& other) const noexcept;
        bool operator!=(const Vector2D& other) const noexcept;
        
        
};

#endif