
#ifndef GAME_BOARD_OBJECT_HPP
#define GAME_BOARD_OBJECT_HPP

//#include <random>

#include "../Utils/utils.hpp"
#include "Vector2D.hpp"
#include "Matrix.hpp"
#include "Pos2D.hpp"

class GameBoardObject {
    public:
        static constexpr size_t num_of_types_of_objs = 5;
        static constexpr char obj_representing_symbols[GameBoardObject::num_of_types_of_objs] = {'-', 'W', 'H', 'B', 'A'};

        Pos2D pos;
        Vector2D direction;
        explicit GameBoardObject(Pos2D arg_pos = Pos2D(0, 0), Vector2D arg_direction = Vector2D(0, 0)) 
            : pos(arg_pos), direction(arg_direction) {
        }
        // Copy constructor
        GameBoardObject(const GameBoardObject& other) : pos(other.pos), direction(other.direction) {}
        // Assignment operator
        GameBoardObject operator=(const GameBoardObject& other);
};

class SnakeSeg : public GameBoardObject {
    public:
        static const unsigned int SnakeSeg::head_representing_num = 2;
        static const unsigned int SnakeSeg::body_representing_num = 3;
        static const char head_representing_symbol = GameBoardObject::obj_representing_symbols[SnakeSeg::head_representing_num];
        static const char body_representing_symbol = GameBoardObject::obj_representing_symbols[SnakeSeg::body_representing_num];
        explicit SnakeSeg(Pos2D arg_pos = Pos2D(0, 0), Vector2D arg_direction = Vector2D(0, 0)) 
        : GameBoardObject(arg_pos, arg_direction) {}
        
};

class Apple : public GameBoardObject {
    public:
        static const unsigned int representing_num = 4;
        static const char representing_symbol = GameBoardObject::obj_representing_symbols[Apple::representing_num];
        explicit Apple(Pos2D arg_pos = Pos2D(0, 0), Vector2D arg_direction = Vector2D(0, 0)) 
        : GameBoardObject(arg_pos, arg_direction) {}
        void randomize_pos(const std::vector<Pos2D> &empty_poses) {
            int random_index = rand() % empty_poses.size();
            this->pos = empty_poses[random_index];
        }
        
};
class Wall : public GameBoardObject {
    public:
        static const unsigned int Wall::representing_num = 1;
        static const char representing_symbol = GameBoardObject::obj_representing_symbols[Wall::representing_num];
        explicit Wall(Pos2D arg_pos = Pos2D(0, 0), Vector2D arg_direction = Vector2D(0, 0)) 
        : GameBoardObject(arg_pos, arg_direction) {}
        
};
class GameBoardObject_Empty {
    public:
        static const unsigned int representing_num = 0;
        static const char representing_symbol = GameBoardObject::obj_representing_symbols[GameBoardObject_Empty::representing_num];
        Pos2D pos;
        explicit GameBoardObject_Empty(Pos2D arg_pos = Pos2D(0, 0)) 
        : pos(arg_pos) {}
};

#endif // GAME_BOARD_OBJECT_HPP