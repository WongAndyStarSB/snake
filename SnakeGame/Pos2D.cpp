#include "../Logger/Logger.hpp"

#include "Pos2D.hpp"

#include "Vector2D.hpp"


// Constructors
Pos2D::Pos2D(int arg_x, int arg_y) noexcept 
    : Vector2D(arg_x, arg_y) {
    //Logger::log("Pos2D(" + std::to_string(arg_x) + ", " + std::to_string(arg_y) + ")\ncreated from arg_x and arg_y", INFO);
}

Pos2D::Pos2D(Vector2D vect) noexcept 
    : Vector2D(vect.x, vect.y) {
    //Logger::log("Pos2D(" + std::to_string(vect.x) + ", " + std::to_string(vect.y) + ")\ncreated from Vector2D", INFO);
}

// copy constructor
Pos2D::Pos2D(const Pos2D& other) noexcept
    : Pos2D(other.x, other.y) {
    //Logger::log("Pos2D(" + std::to_string(other.x) + ", " + std::to_string(other.y) + ")\ncreated from copy", INFO);
}

Pos2D& Pos2D::operator=(const Pos2D& other) noexcept {
    if (this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;
}


Pos2D& Pos2D::operator=(const Vector2D& vect) noexcept {
    x = vect.x;
    y = vect.y;
    return *this;
}


std::string Pos2D::to_string(bool with_prefix) const {
    return (((with_prefix)? ("Pos2D(") : ("(")) + std::to_string(x) + ", " + std::to_string(y) + ")");

}


std::string Pos2D::vector_to_string(const std::vector<Pos2D>& vect, bool with_prefix) {
    std::string return_str = ((with_prefix)? ("vector<Pos2D>[") : ("[")); 
    if (vect.empty()) {
        return return_str+"-empty-]";
    }
    for (size_t i = 0; i < vect.size(); ++i) {
        return_str += vect[i].to_string(false) + ((i == vect.size()-1)? "]" : ", ");
    }
    return return_str;
}


const std::pair<int, int> Pos2D::get_as_pair() const {
    return std::pair<int, int>(x, y);
}





Pos2D Pos2D::operator+(const Vector2D& vect) const noexcept {
    return Pos2D(x+vect.x, y+vect.y);
}
Pos2D Pos2D::operator-(const Vector2D& vect) const noexcept {
    return Pos2D(x-vect.x, y-vect.y);
}
Pos2D Pos2D::operator+(const Pos2D& other) const noexcept {
    return Pos2D(x+other.x, y+other.y);
}
Pos2D Pos2D::operator-(const Pos2D& other) const noexcept {
    return Pos2D(x-other.x, y-other.y);
}
Pos2D Pos2D::operator%(const Pos2D& other) const noexcept {
    return Pos2D(x%other.x, y%other.y);
}
bool Pos2D::operator<(const Pos2D& other) const noexcept {
    return (x < other.x) && (y < other.y);
}
bool Pos2D::operator>(const Pos2D& other) const noexcept {
    return (x > other.x) && (y > other.y);
}
bool Pos2D::operator==(const Pos2D& other) const noexcept {
    return (x == other.x) && (y == other.y);
}
bool Pos2D::operator!=(const Pos2D& other) const noexcept {
    return !(*this == other);
}
bool Pos2D::operator<=(const Pos2D& other) const noexcept {
    return (*this < other) || (*this == other);
}
bool Pos2D::operator>=(const Pos2D& other) const noexcept {
    return (*this > other) || (*this == other);
}