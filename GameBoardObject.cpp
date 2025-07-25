#include "GameBoardObject.hpp"

//public member functions
GameBoardObject GameBoardObject::operator=(const GameBoardObject& other) {
    if (this != &other) {
        this->pos = other.pos;
        this->direction = other.direction;
    }
    return *this;
}
// void GameBoardObject::change_values_as(const GameBoardObject& other) {
//     this->pos = other.pos;
//     this->direction = other.direction;
// }




