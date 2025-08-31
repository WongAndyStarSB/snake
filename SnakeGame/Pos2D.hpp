#ifndef POS2D_HPP
#define POS2D_HPP

#include "Vector2D.hpp"

#include <vector>
#include <string>

class Pos2D: public Vector2D {
    public:
        explicit Pos2D(int arg_x, int arg_y) noexcept;
        explicit Pos2D(Vector2D vect) noexcept;

        inline explicit Pos2D() noexcept : Vector2D(0, 0) {}

        inline Pos2D(int arr[2]) noexcept : Vector2D(arr) {}
        
        // copy constructor
        Pos2D(const Pos2D& other) noexcept;
        // assignment operator
        Pos2D& operator=(const Pos2D& other) noexcept;
        // assignment operator with Vector2D
        Pos2D& operator=(const Vector2D& vect) noexcept;

        // other methods
        std::string to_string(bool with_prefix = true) const;
        static std::string vector_to_string(const std::vector<Pos2D>& vect, bool with_prefix = true);
        const std::pair<int, int> get_as_pair() const;

        // delete some useless member functions from Vector2D

        static Vector2D get_zero_vector() noexcept = delete;
        static Vector2D get_up_vector() noexcept = delete;
        static Vector2D get_down_vector() noexcept = delete;
        static Vector2D get_left_vector() noexcept = delete;
        static Vector2D get_right_vector() noexcept = delete;

        bool is_same_direction_with(const Vector2D&, bool) const noexcept = delete;
        bool is_opposite_direction_with(const Vector2D&, bool) const noexcept = delete;
        bool is_parallel_with(const Vector2D&, bool) const noexcept = delete;
        

        // delete the following operators to prevent using them
        // Vector2D operator+(const Vector2D& other) const = delete;
        // Vector2D operator-(const Vector2D& other) const = delete;
        // Vector2D operator%(const Vector2D& other) const = delete;
        bool operator<(const Vector2D& other) const = delete;
        bool operator>(const Vector2D& other) const = delete;
        bool operator==(const Vector2D& other) const = delete;
        bool operator!=(const Vector2D& other) const = delete;
        bool operator<=(const Vector2D& other) const = delete;
        bool operator>=(const Vector2D& other) const = delete;

        // Pos2D specific operators
        Pos2D operator+(const Vector2D& vect) const noexcept;
        Pos2D operator-(const Vector2D& vect) const noexcept;
        Pos2D operator+(const Pos2D& other) const noexcept;
        Pos2D operator-(const Pos2D& other) const noexcept;
        Pos2D operator%(const Pos2D& other) const noexcept;
        bool operator<(const Pos2D& other) const noexcept;
        bool operator>(const Pos2D& other) const noexcept;
        bool operator==(const Pos2D& other) const noexcept;
        bool operator!=(const Pos2D& other) const noexcept;
        bool operator<=(const Pos2D& other) const noexcept;
        bool operator>=(const Pos2D& other) const noexcept;
        

};

namespace std {
    template <>
    struct hash<Pos2D> {
        std::size_t operator()(const Pos2D& p) const noexcept {
            return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
        }
    };
}

#endif // POS2D_HPP
// End of file