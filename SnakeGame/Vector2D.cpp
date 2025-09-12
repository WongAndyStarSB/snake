#include "Vector2D.hpp"

#include <vector>
#include <string>
#include <typeinfo>

#include "../Utils/utils.hpp"
#include "../Math/Fraction.hpp"

bool Vector2D::isVectorInRange(int upper_lim_x, int upper_lim_y, int lower_lim_x, int lower_lim_y) {
    return (
        Utils::Int::is_int_in_range(this->x, upper_lim_x, lower_lim_x) 
        && Utils::Int::is_int_in_range(this->y, upper_lim_y, lower_lim_y));
}

bool Vector2D::isVectorInRange(Vector2D upper_lim, Vector2D lower_lim) {
    if (lower_lim.x > upper_lim.x || lower_lim.y > upper_lim.y) {
        throw std::runtime_error("lower_lim cannot be larger than upper_lim");
    }
    return (this->x <= upper_lim.x) && (this->y <= upper_lim.y) 
        && (this->x >= lower_lim.x) && (this->y >= lower_lim.y);
}

std::string Vector2D::to_string(bool with_prefix) const {
    return (((with_prefix)? (Utils::get_class_name(this)) : ("")) + "(" + std::to_string(x) + ", " + std::to_string(y) + ")");
}
std::array<int, 2> Vector2D::to_array() const {
    return {x, y};
}
std::array<double, 2> Vector2D::to_array_double() const {
    return {static_cast<double>(x), static_cast<double>(y)};
}


std::string Vector2D::vector_to_string(const std::vector<Vector2D>& vect, bool with_prefix) {
    std::string return_str = ((with_prefix)? ("vector<Vector2D>[") : ("[")); 
    for (size_t i = 0; i < vect.size(); ++i) {
        return_str += vect[i].to_string(false) + ((i == vect.size()-1)? "]" : ", ");
    }
    return return_str;
}


Vector2D Vector2D::get_zero_vector() noexcept {
    return Vector2D(0, 0);
}
Vector2D Vector2D::get_up_vector() noexcept {
    return Vector2D(0, -1);
}
Vector2D Vector2D::get_down_vector() noexcept {
    return Vector2D(0, 1);
}
Vector2D Vector2D::get_left_vector() noexcept {
    return Vector2D(-1, 0);
}
Vector2D Vector2D::get_right_vector() noexcept {
    return Vector2D(1, 0);
}

void Vector2D::change_value_as(int new_x, int new_y) {
    x = new_x;
    y = new_y;
}
void Vector2D::change_value_as(Vector2D other) {
    x = other.x;
    y = other.y;
}

bool Vector2D::is_same_direction_with(const Vector2D& other, bool zero_vector_consider_as_true) const noexcept {
    if (x == 0 && y == 0 && other.x == 0 && other.y == 0) {
        return true;
    }
    if ((x == 0 && y == 0) || (other.x == 0 && other.y == 0)) {
        return zero_vector_consider_as_true;
    }
    if (x == 0 && other.x == 0) {
        return y*other.y > 0; // vertically same direction
    }
    if (y == 0 && other.y == 0) {
        return x*other.x > 0; // horizontally same direction
    }
    if (x == 0 || y == 0 || other.x == 0 || other.y == 0) {
        return false; // not parallel
    }
    NS_math::Fraction x_ratio(x, other.x);
    return x_ratio > 0 && x_ratio == NS_math::Fraction(y, other.y);
}
bool Vector2D::is_opposite_direction_with(const Vector2D& other, bool zero_vector_consider_as_true) const noexcept {
    if (x == 0 && y == 0 && other.x == 0 && other.y == 0) {
        return false;
    }
    if ((x == 0 && y == 0) || (other.x == 0 && other.y == 0)) {
        return zero_vector_consider_as_true;
    }
    if (x == 0 && other.x == 0) {
        return y*other.y < 0; // vertically opposite
    }
    if (y == 0 && other.y == 0) {
        return x*other.x < 0; // horizontally opposite
    }
    if (x == 0 || y == 0 || other.x == 0 || other.y == 0) {
        return false; // not parallel
    }
    NS_math::Fraction x_ratio {x, other.x};
    return x_ratio < 0 && x_ratio == NS_math::Fraction(y, other.y);
}
bool Vector2D::is_parallel_with(const Vector2D& other, bool zero_vector_consider_as_true) const noexcept {
    if (x == 0 && y == 0 && other.x == 0 && other.y == 0) {
        return true;
    }
    if ((x == 0 && y == 0) || (other.x == 0 && other.y == 0)) {
        return zero_vector_consider_as_true;
    }
    if ((x == 0 && other.x == 0) || (y == 0 && other.y == 0)) {
        return true; // both vertical or both horizontal
    }
    if (x == 0 || y == 0 || other.x == 0 || other.y == 0) {
        return false; // not parallel
    }
    return NS_math::Fraction(x, other.x) == NS_math::Fraction(y, other.y);
}


Vector2D Vector2D::operator+(const Vector2D& other) const noexcept {
    return Vector2D(this->x+other.x, this->y+other.y);
}
Vector2D Vector2D::operator-(const Vector2D& other) const noexcept {
    return Vector2D(this->x-other.x, this->y-other.y);
}
Vector2D Vector2D::operator*(const int& scalar) const noexcept {
    return Vector2D(x*scalar, y*scalar);
}
Vector2D Vector2D::operator*(const double& scalar) const noexcept {
    return Vector2D(static_cast<int>(x * scalar), static_cast<int>(y * scalar)); 
}

Vector2D& Vector2D::operator+=(const Vector2D& other) noexcept {
    x += other.x;
    y += other.y;
    return *this;
}
Vector2D& Vector2D::operator-=(const Vector2D& other) noexcept {
    x -= other.x;
    y -= other.y;
    return *this;
}
Vector2D& Vector2D::operator*=(const int& scalar) noexcept {
    x *= scalar;
    y *= scalar;
    return *this;
}
Vector2D& Vector2D::operator*=(const double& scalar) noexcept {
    x = static_cast<int>(scalar * x);
    y = static_cast<int>(scalar * x);
    return *this;
}
int Vector2D::dot_mul(const Vector2D& other) const noexcept {
    return (this->x*other.x, this->y*other.y);
}
int Vector2D::det(const Vector2D& other) const noexcept {
    return (this->x * other.y) - (this->y * other.x);
}

bool Vector2D::operator==(const Vector2D& other) const noexcept {
    return (this->x == other.x) && (this->y == other.y);
}
bool Vector2D::operator!=(const Vector2D& other) const noexcept {
    return !(*this == other);
}
