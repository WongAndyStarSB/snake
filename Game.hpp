#ifndef GAME_HPP
#define GAME_HPP


#include <ostream>
#include <memory>
#include <vector>
#include <string>
#include <queue>

#include "Vector2D.hpp"
#include "Matrix.hpp"
#include "Size2D.hpp"
#include "Level.hpp"
#include "GameBoardObjects.hpp"

enum GameStopReason {
    NOT_STOPPING = -1,
    PREPARING = 0,
    PAUSED,
    LOSED,
    WON
};
enum GameStatus {
    STOP = 0,
    RUNNING = 1
};

class Game {
    private:
        bool init_done = false;
        GameStatus status = STOP;
        GameStopReason stop_reason = PREPARING;
        std::unique_ptr<GameBoardObjects> game_board_objects = nullptr;
        std::queue<Vector2D> snake_velocity_queue;

        
        void start_game();
        void stop_game(GameStopReason reason);

        void add_velocity_to_queue(Vector2D velocity);
        void move_snake(bool force = false);
        void display(std::ostream& os, int n = 0) const;

        void throw_if_init_not_done(const std::string& method_name = "", const std::string other_info = "") const;
        std::string add_prefix_and_indent_for_log(const std::string& message, bool step_and_snake_pos_prefix) const;
        
        
    public:
        
        size_t num_of_step = 0;
        Size2D board_size;
        Matrix<int> board2d;
        Level level;
        unsigned int snake_period_in_frame_per_square = 6;
        
        
        // explicit Game(
        //     Size2D arg_board_size, 
        //     std::string arg_level_id = "T000", 
        //     Pos2D arg_snake_init_pos = Pos2D(0, 0), 
        //     size_t arg_apple_init_num = 1);
        
        explicit Game(const Level& arg_level);

        Game(const Game&) = delete; // disable copy constructor
        Game& operator=(const Game&) = delete; // disable copy assignment
        Game(Game&&) = default; // enable move constructor

        void init(std::string new_lev_id = "");
        void update(size_t frame_num, Vector2D next_snake_velocity, std::ostream& os);
        void pause();
        void resume();
        void start();
        void restart(std::string new_lev_id = "");
        void lose();
        void win();

        

        
        GameStatus get_status();
        GameStopReason get_stop_reason();

        void log(const std::string& message, LogLevel lev, bool step_and_snake_pos_prefix = true) const;
        void log_and_throw(const std::string& message, bool step_and_snake_pos_prefix = true) const;

        static std::string game_stop_reason_to_string(const GameStopReason& reason);
};


#endif