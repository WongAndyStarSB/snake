#include "Game.hpp"



#include <stdexcept>
#include <ctime>
#include "StringUtils.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <chrono>


#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>

#include "StringUtils.hpp"
#include "Logger.hpp"



// public

// Game::Game (
//     Size2D arg_board_size, 
//     std::string arg_level_id, 
//     Pos2D arg_snake_init_pos, 
//     size_t arg_apple_init_num
//     ) : board_size(arg_board_size), 
//         board2d(arg_board_size.y, arg_board_size.x, 0), 
//         level(Level::create_and_register(arg_level_id, board2d, arg_snake_init_pos, arg_apple_init_num)),
//         game_board_objects(nullptr) // tmp, change later
//     {
//     Logger::log_and_throw("This constructor is not usable yet");
//     board2d[arg_snake_init_pos.y][arg_snake_init_pos.x] = SnakeSeg::head_representing_num;
// }

Game::Game (
    const Level& arg_level
    ) : board_size(arg_level.get_board().num_of_col, arg_level.get_board().num_of_row), 
        board2d(arg_level.get_board()), 
        level(arg_level), 
        game_board_objects(nullptr)
    {
}
// copy constructor
// Game::Game(const Game&) = delete; // disable copy constructor
// assignment operator
// Game& Game::operator=(const Game&) = delete; // disable copy assignment
// move constructor
// Game::Game(Game&&) = default; // enable move constructor

void Game::init(
    SDL_Window* w, SDL_Renderer* r, 
    std::ostream& os_, 
    std::string new_lev_id
) {
    Logger::log(
        "Game::init(SDL_Window* w, SDL_Renderer* r, std::ostream& os, std::string new_lev_id", 
        "initializing", 
        Logger::INFO
    );
    if (window == nullptr || renderer == nullptr) {
        log_and_throw<std::runtime_error>(
            "Game::init(SDL_Window* w, SDL_Renderer* r, std::ostream& os, std::string new_lev_id",
            "window or renderer is nullptr"
        );
    } else {
        window = w;
        renderer = r;
    }
    os = &os_;
    init_lev(new_lev_id);
}
void Game::init_lev(std::string new_lev_id) {
    if (new_lev_id != "") {
        level = Level::find_level(new_lev_id).get_copy();
    }
    this->num_of_step = 0;
    this->time_used_in_s = 0;
    this->frame_num = 0;
    this->status = STOP;
    this->stop_reason = GameStopReason::PREPARING;
    this->player_direction = Vector2D::get_zero_vector();

    board2d = level.get_board_reference();
    board2d[level.get_snake_init_pos_reference().y][level.get_snake_init_pos_reference().x] = SnakeSeg::head_representing_num;
    // delete game_board_objects (no need as it is unique_ptr)
    game_board_objects = std::make_unique<GameBoardObjects>(this);
    game_board_objects->init();

    
    snake_direction = Vector2D(0, 0); // initial velocity is (0, 0)

    init_done = true;
}

GameStatus Game::get_status() {
    return status;
}
GameStopReason Game::get_stop_reason() {
    return stop_reason;
}


void Game::run() {
    log("run()", "function started", Logger::INFO);
    display();
    std::array<int, 3> start_time = {0, 0, 0};
    SDL_Event event;

    std::chrono::high_resolution_clock::time_point tmp_start, tmp_end;
    unsigned int tmp_duration; // in microseconds
    start_moving();
    while (true) {
        tmp_start = std::chrono::high_resolution_clock::now();
        if (this->status != RUNNING) {
            start_time = Utils::Time::get_current_hour_min_sec();
        }
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                return;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.scancode == SDL_SCANCODE_W || event.key.scancode == SDL_SCANCODE_UP || event.key.key == SDLK_KP_8) {
                    start_moving();
                    player_direction = Vector2D::get_up_vector();
                } else if (event.key.scancode == SDL_SCANCODE_S || event.key.scancode == SDL_SCANCODE_DOWN || event.key.key == SDLK_KP_5) {
                    start_moving();
                    player_direction = Vector2D::get_down_vector();
                } else if (event.key.scancode == SDL_SCANCODE_A || event.key.scancode == SDL_SCANCODE_LEFT || event.key.key == SDLK_KP_4) {
                    start_moving();
                    player_direction = Vector2D::get_left_vector();
                } else if (event.key.scancode == SDL_SCANCODE_D || event.key.scancode == SDL_SCANCODE_RIGHT || event.key.key == SDLK_KP_6) {
                    start_moving();
                    player_direction = Vector2D::get_right_vector();
                } else if (event.key.scancode == SDL_SCANCODE_SPACE) {
                    if (status == RUNNING) {
                        pause();
                    } else if (stop_reason == PAUSED) {
                        resume();
                    }
                } else if (event.key.scancode == SDL_SCANCODE_R) {
                    if (status == STOP && stop_reason != PAUSED) {
                        player_direction = Vector2D::get_zero_vector();
                        restart();
                    }
                }
            }
        }
        
        
        if (status == RUNNING) {
            update(player_direction);
            time_used_in_s = 
                Utils::Time::time_minus_get_seconds(
                    Utils::Time::get_current_hour_min_sec(), start_time
                )
            ;
            ++frame_num;
        }
        
        tmp_end = std::chrono::high_resolution_clock::now();
        tmp_duration = static_cast<unsigned int>((std::chrono::duration_cast<std::chrono::microseconds>(tmp_end - tmp_start)).count());
        
        log("", std::to_string(frame_num % snake_period_in_frame_per_square == 0) + " " + std::to_string(tmp_duration) + "µs", Logger::DEBUG);

        
        if (tmp_duration < MICROS_PER_FRAME) {
            SDL_Delay((MICROS_PER_FRAME - tmp_duration) / 1000);
        } else {
            log("run()", "tmp_duration >= MICROS_PER_FRAME", Logger::LogLevel::WARNING_1);
        }
        
        
        if (status == STOP) {
            start_time = Utils::Time::get_current_hour_min_sec();
        }
        // if (num_of_step == 10) {
        //     Logger::log_and_throw("", "stop");
        // } 
    }
}


void Game::add_velocity_to_queue(Vector2D velocity) {
    if (
        velocity.is_opposite_direction_with(snake_direction, false)
        && game_board_objects->get_snake_length() > 1) {
        ;
    } else {
        snake_direction = velocity;
    }
}

void Game::update(Vector2D next_snake_velocity) {
    throw_if_init_not_done("update(Vector2D next_snake_velocity, std::ostream& os)");
    if (status == STOP) {
        log("update", "update skipped as game have been stopped", Logger::INFO);
        return;
    }
    
    if (frame_num % snake_period_in_frame_per_square == 0) {
        snake_direction = next_snake_velocity;
        move_snake();
        if (status == STOP) {
            return;
        }
    }
    
    // Logger::log("clear_terminal", 
    //     std::to_string(Utils::Time::duration_used_in_function()),
    //     Logger::DEBUG
    // );
    cliClearScreen();
    display(
        (frame_num % snake_period_in_frame_per_square) * 3 / snake_period_in_frame_per_square
    ); // display the game board
}

void Game::pause() {
    stop_game(PAUSED);
}
void Game::resume() {
    if (status != STOP || stop_reason != PAUSED) {
        log("resume()", "game is not paused, cannot resume", Logger::WARNING_2);
        return;
    }
    start_game();
}
void Game::start() {
    start_game();
}
void Game::restart(std::string new_lev_id) {
    init_lev(new_lev_id);
    
    start_game();
}
void Game::lose() {
    stop_game(LOSED);
    Utils::Time::delay_for_time_in_ms(1000);
    std::cout << "\n\nYou Losed!\n";
    record(
        "Game Losed\nlevel: " + level.get_id() 
        + "\nsnake_length: " + std::to_string(game_board_objects->get_snake_length())
        + "\nstep_no.: " + std::to_string(this->num_of_step)
        + "\ntime(s): " + std::to_string(this->time_used_in_s)
    );
}
void Game::win() {
    stop_game(WON);
}


void Game::move_snake(bool force) {
    throw_if_init_not_done("move_snake");
    if (status == STOP) {
        log("move_snake(bool force)", "move_snake skipped as game have been stopped", Logger::INFO);
        return;
    }
    if (snake_direction == Vector2D(0, 0)) {
        log("move_snake(bool force)", "move_snake skipped as direction == (0, 0)", Logger::INFO);
        return;
    }
    try {
        if (force) {
            game_board_objects->force_update(snake_direction);
        } else {
            game_board_objects->update(snake_direction);
        }
        ++num_of_step;
    } catch (std::runtime_error e) {
        log_and_throw<Logger::SeeAbove>("move_snake(bool force)", e.what());
    }
    
    
}


// private
void Game::display(int n) const {
    throw_if_init_not_done("display(int n)");
    
    Matrix<char> display_str_matrix(board_size.y, board_size.x*3, ' ');
    int col_num_of_str;
    const Snake& snake = game_board_objects->get_snake();
    const std::vector<Apple>& apples = game_board_objects->get_apples();
    const std::vector<Wall>& walls = game_board_objects->get_walls();
    const std::vector<Pos2D>& empty_poses = game_board_objects->get_empty_poses();
    const auto snake_head_iterator = snake.get_head_iterator();
    Vector2D left_vector = Vector2D::get_left_vector();
    Vector2D right_vector = Vector2D::get_right_vector();
    int tmp;
    for (const Apple& apple : apples) {
        auto [c, r] = apple.pos.get_as_pair();
        display_str_matrix[r][c*3+1] = Apple::representing_symbol;
    }
    for (const Wall& wall : walls) {
        auto [c, r] = wall.pos.get_as_pair();
        display_str_matrix[r][c*3+1] = Wall::representing_symbol;
    }
    for (const Pos2D& pos : empty_poses) {
        auto [c, r] = pos.get_as_pair();
        display_str_matrix[r][c*3+1] = '-';
    }
    
    for (auto it = snake.snake_segments.begin(); it != snake.snake_segments.end(); ++it) {
        auto [c, r] = (*it)->pos.get_as_pair();
        col_num_of_str = c*3+1;
        if (it == snake.get_head_iterator()) {
            display_str_matrix[r][col_num_of_str] = SnakeSeg::head_representing_symbol;
            tmp = n;
        } else {
            display_str_matrix[r][col_num_of_str] = SnakeSeg::body_representing_symbol;
            tmp = 2;
        }
        for (int i = 1; i <= tmp; ++i) {
            if ((*it)->direction == left_vector && col_num_of_str-i >= 0) {
                display_str_matrix[r][col_num_of_str-i] = '<';
            } else if ((*it)->direction == right_vector && col_num_of_str+i < display_str_matrix.num_of_col) {
                display_str_matrix[r][col_num_of_str+i] = '>';
            }
        }
    }
    (*os) << display_str_matrix.join_into_string("\n", "")
       << "\n\nlevel: " << level.get_id() 
       << "\nsnake_length: " << std::to_string(game_board_objects->get_snake_length())
       << "\nstep_no.: " << std::to_string(this->num_of_step)
       << "\ntime(s): " << std::to_string(this->time_used_in_s)
       << "\nframe_num: " << std::to_string(this->frame_num)
    ;

}

void Game::record(const std::string& message, bool add_timestamp) {
    std::string msg = StringUtils::add_indent(message, 4);
    std::time_t now = std::time(nullptr);
    std::tm local_time;
    #ifdef _MSC_VER
        localtime_s(&local_time, &now);
    #else
        local_time = *std::localtime(&now);
    #endif
    std::string year_str = std::to_string(local_time.tm_year + 1900);
    std::string month_and_day = std::to_string(local_time.tm_mon + 1) + "-" +
                                std::to_string(local_time.tm_mday);
    if (add_timestamp) {
        std::string timestamp = std::to_string(local_time.tm_hour) + ":" +
                                std::to_string(local_time.tm_min) + ":" +
                                std::to_string(local_time.tm_sec);
        msg = "[" + timestamp + "] {\n" + msg + "\n}";
    } else {
        msg = "[] {\n" + msg + "\n}";
    }
    
    namespace fs = std::filesystem;
    fs::path log_path = "GameRecords\\" + year_str + "\\" + month_and_day + ".txt";
    fs::create_directories(log_path.parent_path());
    std::ofstream log_file(log_path, std::ios_base::app);
    if (log_file.is_open()) {
        log_file << msg << std::endl;
    } else {
        std::cerr << "Unable to open log file." << std::endl;
    }
    log_file.flush();
}

void Game::cliClearScreen() const {
    (*os) << std::string(30, '\n');
}


// static

std::string Game::game_stop_reason_to_string(const GameStopReason& reason) {
    switch (reason) {
        case PAUSED:
            return "PAUSED";
        case LOSED:
            return "LOSED";
        case WON:
            return "WON";
        default:
            Logger::log_and_throw<std::logic_error>(
                "Game::game_stop_reason_to_string", "NEVER"
            );
    }
}

std::string Game::game_status_to_string(const GameStatus& status) {
    switch (status) {
        case STOP:
            return "STOP";
        case RUNNING:
            return "RUNNING";
        default:
            Logger::log_and_throw<std::logic_error>(
                "Game::game_status_to_string", "NEVER"
            );
    }
}

// private

void Game::start_game() {
    // log("start_game()", "", Logger::INFO);
    throw_if_init_not_done("start_game()");
    if (status == RUNNING) {
        log("start_game()", "game is already running, no need to start", Logger::WARNING_2);
        return;
    }
    
    run();
}
void Game::stop_game(GameStopReason reason) {
    // log("stop_game(GameStopReason reason)", "reason:" + game_stop_reason_to_string(reason), Logger::INFO);
    status = STOP;
    stop_reason = reason;
}

void Game::start_moving() {
    // throw_if_init_not_done("start_run");
    status = RUNNING;
    stop_reason = NOT_STOPPING;
}

void Game::throw_if_init_not_done(const std::string& method_name, const std::string other_info) const {
    if (!init_done) {
        Logger::log_and_throw<std::domain_error>(
            "Game::" + method_name, 
            "ShouldNotBeCalled: the initialization of this has not done" 
                + ((other_info == "")? "" : "\n"+other_info)
        );
    }
}

std::string Game::add_prefix_and_indent_for_log(const std::string& message, bool step_and_snake_pos_prefix) const {
    std::string prefix = "[";
    if (step_and_snake_pos_prefix) {
        // prefix += 
        //     "StepNo.:" 
        //     + std::to_string(num_of_step) 
        //     + " SnakeHeadPos:" 
        //     + (
        //         (game_board_objects == nullptr || !game_board_objects->init_done)? 
        //          "(game_board_objects have not initialized)" 
        //         : (game_board_objects->get_snake()->head->pos.to_string(false))
        //     )
        // ;
        prefix += 
            "Status:"
            + game_status_to_string(status)
            + " StepNo.:" 
            + std::to_string(num_of_step) 
            + " frame_num:" 
            + std::to_string(frame_num)
            + " SnakeHeadPos:"
        ;
        if (game_board_objects == nullptr || !game_board_objects->init_done) {
            prefix += "/* game_board_objects have not initialized */";
        } else {
            prefix += 
                (game_board_objects->get_snake().head->pos.to_string(false)) 
                + " snake_length:" 
                + std::to_string(game_board_objects->get_snake_length());
        }
        
    }
    prefix += " ]\n";
    return prefix + StringUtils::add_indent(message, 2);
}