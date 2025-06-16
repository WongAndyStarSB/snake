#ifndef SIZE2D_HPP
#define SIZE2D_HPP

#include <exception>


class InvalidArgumentError : public std::exception {
    private:
        std::string message;
    public:
        InvalidArgumentError(const char* arg_message)
        : message(arg_message) {}

        const char* what() const throw() {
            return message.c_str();
        }
};

class Size2D {
    public:
        size_t x;
        size_t y;
        explicit Size2D(size_t arg_x, size_t arg_y) : x(arg_x), y(arg_y) {
            if (arg_x < 0 || arg_y < 0) {
                throw InvalidArgumentError("Size2D::Size2D: x and y should be non-negative");
            }
        }
        explicit Size2D(Vector2D vect) : x(vect.x), y(vect.y) {
            if (vect.x < 0 || vect.y < 0) {
                throw InvalidArgumentError("Size2D::Size2D: vect.x and vect.y should be non-negative");
            }
        }
        // copy constructor
        Size2D(const Size2D& other) : x(other.x), y(other.y) {}
        // assignment operator
        Size2D& operator=(const Size2D& other) {
            if (this != &other) {
                this->x = other.x;
                this->y = other.y;
            }
            return *this;
        }
        // assignment operator with Vector2D
        Size2D& operator=(Vector2D vect) {
            if (vect.x < 0 || vect.y < 0) {
                throw InvalidArgumentError("Size2D::operator=: vect.x and vect.y should be non-negative");
            }
            this->x = vect.x;
            this->y = vect.y;
            return *this;
        }
        std::string to_string() const {
            return ("Size2D(" + std::to_string(x) + ", " + std::to_string(y) + ")");
        }
        bool operator==(const Size2D& other) const {
            return (this->x == other.x) && (this->y == other.y);
        }
        bool operator!=(const Size2D& other) const {
            return !(*this == other);
        }
        bool operator<(const Size2D& other) const {
            return (this->x < other.x) && (this->y < other.y);
        }
        bool operator>(const Size2D& other) const {
            return (this->x > other.x) && (this->y > other.y);
        }
        bool operator<=(const Size2D& other) const {
            return (*this < other) || (*this == other);
        }
        bool operator>=(const Size2D& other) const {
            return (*this > other) || (*this == other);
        }
};


#endif