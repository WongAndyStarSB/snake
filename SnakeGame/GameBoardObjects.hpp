#ifndef GAME_BOARD_OBJECT_S_HPP
#define GAME_BOARD_OBJECT_S_HPP

#include <iostream>
#include <memory>
#include <algorithm>
#include <cassert>
#include <vector>

#include "ostream_overloads.hpp"

#include "utils.hpp"
#include "Logger.hpp"
#include "Vector2D.hpp"
#include "Pos2D.hpp"
#include "Size2D.hpp"
#include "Matrix.hpp"
#include "Level.hpp"
#include "GameBoardObject.hpp"
#include "Snake.hpp"

class Game; // forward declaration


class GameBoardObjects {
  private:
    std::unique_ptr<Snake> snake;
    std::vector<Apple> apples;
    std::vector<Wall> walls;
    std::vector<Pos2D> empty_poses;
    //std::vector<GameBoardObject_Empty> empties;

    size_t snake_length = 1;

    
    void throw_if_init_not_done(const std::string& method_name, const std::string other_info = "") const;
      // board
    void update_board(Matrix<int>* tmp_board = nullptr);
    // snake
    void snake_move(const Vector2D& next_snake_direction);
    void snake_grow();
    // apple
    void apple_randomize_pos(Apple &apple, bool eaten_by_snake = false);
    // wall
    
    // empty_poses
    void update_empty_poses(Pos2D* top_left_of_range = nullptr, Size2D* size_of_range = nullptr);
    std::vector<Pos2D>::iterator empty_poses_find(Pos2D pos);
    std::vector<Pos2D>::iterator empty_poses_remove(const Pos2D &pos_to_remove);
    std::vector<Pos2D> empty_poses_remove(std::vector<Pos2D> poses_to_remove);
  
  public:
    bool init_done = false;

    
    Game* related_game;

    explicit GameBoardObjects(Game* arg_related_game);

    GameBoardObjects(const GameBoardObjects&) = delete; // disable copy constructor
    GameBoardObjects& operator=(const GameBoardObjects&) = delete; // disable copy assignment
    GameBoardObjects(GameBoardObjects&&) = default; // enable move constructor

    void init();
    void update(Vector2D next_snake_direction);
    void force_update(const Vector2D& next_snake_velocity);

    const Snake& get_snake() const;
    Snake get_snake_copy() const;
    const std::vector<Apple>& get_apples() const;
    const std::vector<Wall>& get_walls() const;
    const std::vector<Pos2D>& get_empty_poses() const;

    size_t get_snake_length() const;

  private:

    void log(const std::string& where, const std::string& message, const Logger::LogLevel& lev) const;
    
    template <typename ExceptionType>
    [[noreturn]] void log_and_throw(const std::string& where, const std::string& message) const {
      related_game->log_and_throw<ExceptionType>(
        "GameBoardObjects::" + where, 
        message);
    }
};
    

#endif // GAME_BOARD_OBJECT_S_HPP