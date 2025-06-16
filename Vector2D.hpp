#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP


#include <vector>
#include <string>

class Vector2D {
    public:
        int x;
        int y;
        explicit Vector2D(int arg_x, int arg_y) noexcept : x(arg_x), y(arg_y) {}
        bool isVectorInRange(int upper_lim_x, int upper_lim_y, int lower_lim_x = 0, int lower_lim_y = 0);
        bool isVectorInRange(Vector2D upper_lim, Vector2D lower_lim = Vector2D(0, 0));

        std::string to_string(bool with_prefix = true) const ;
        static std::string vector_to_string(const std::vector<Vector2D>& vect, bool with_prefix = true) ;

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
        Vector2D operator%(const Vector2D& other) const noexcept;
        bool operator<(const Vector2D& other) const noexcept;
        bool operator>(const Vector2D& other) const noexcept;
        bool operator==(const Vector2D& other) const noexcept;
        bool operator!=(const Vector2D& other) const noexcept;
        bool operator<=(const Vector2D& other) const noexcept;
        bool operator>=(const Vector2D& other) const noexcept;
        
};

#endif