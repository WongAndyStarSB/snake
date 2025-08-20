#ifndef GAME_HPP
#define GAME_HPP


#include <ostream>
#include <memory>
#include <vector>
#include <string>
#include <queue>

#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_events.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "StringUtils.hpp"
#include "Math/Fraction.hpp"
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
enum GameRunStatus {
    _PAUSED,
    FINISHED
};

class Game {
    private:
        bool init_done = false;
        GameStatus status = STOP;
        GameStopReason stop_reason = PREPARING;

        SDL_Window* window; // non-owning // don't delete
        SDL_Renderer* renderer; // non-owning // don't delete
        Vector2D player_direction = Vector2D::get_zero_vector();
        size_t num_of_step = 0;
        Vector2D snake_direction = Vector2D::get_zero_vector();
        std::unique_ptr<GameBoardObjects> game_board_objects = nullptr;
        unsigned int time_used_in_s = 0;

        unsigned int snake_velocity_in_square_per_ks = 2000;

        unsigned int frame_num = 0;
        const uint8_t FRAME_RATE = 30;
        const Math::Fraction MICROS_PER_FRAME_FRACTION {(int)1000000, static_cast<int>(FRAME_RATE)}; // 1000000 microseconds in a second divided by frame rate
        const unsigned int MICROS_PER_FRAME = MICROS_PER_FRAME_FRACTION.floor(); // Convert to milliseconds

        
        void start_game();
        void stop_game(GameStopReason reason);
        void start_moving();

        void add_velocity_to_queue(Vector2D velocity);
        void move_snake(bool force = false);
        void run();
        void display(std::ostream& os, int n = 0) const;
        void record(const std::string& message , bool add_timestamp = true);
        void cliClearScreen() const;

        void throw_if_init_not_done(const std::string& method_name = "", const std::string other_info = "") const;
        std::string add_prefix_and_indent_for_log(const std::string& message, bool step_and_snake_pos_prefix) const;
        
        
    public:
        
        
        Size2D board_size;
        Matrix<int> board2d;
        Level level;
        unsigned int snake_period_in_frame_per_square = FRAME_RATE * 1000 / snake_velocity_in_square_per_ks;
        
        
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
        void start();
        void restart(std::string new_lev_id = "");

        void update(Vector2D next_snake_velocity, std::ostream& os);
        void pause();
        void resume();
        
        
        void lose();
        void win();

        

        
        GameStatus get_status();
        GameStopReason get_stop_reason();

        void log(const std::string& where, const std::string& message, Logger::LogLevel lev, bool step_and_snake_pos_prefix = true) const {
            
            std::string msg = add_prefix_and_indent_for_log(message, step_and_snake_pos_prefix);
            Logger::log("Game::" + where, msg, lev);
        }
        template <typename ExceptionType>
        [[noreturn]] void log_and_throw(const std::string& where, const std::string& message, bool step_and_snake_pos_prefix = true) const {
            
            std::string msg = add_prefix_and_indent_for_log(message, step_and_snake_pos_prefix);
            Logger::log_and_throw<ExceptionType>("Game::" + where, msg);
        }

        static std::string game_stop_reason_to_string(const GameStopReason& reason);
        static std::string game_status_to_string(const GameStatus& status);
};


#endif