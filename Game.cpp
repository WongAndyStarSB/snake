#include "Game.hpp"



#include <stdexcept>

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
//Game::Game(const Game&) = delete; // disable copy constructor
// assignment operator
// Game& Game::operator=(const Game&) = delete; // disable copy assignment
// move constructor
// Game::Game(Game&&) = default; // enable move constructor

void Game::init(std::string new_lev_id) {
    Logger::log(
        "Game::init(std::string new_lev_id)", 
        "initializing", 
        Logger::INFO
    );
    if (new_lev_id != "") {
        level = Level::find_level(new_lev_id).get_copy();
    }
    num_of_step = 0;
    board2d = level.get_board_reference();
    board2d[level.get_snake_init_pos_reference().y][level.get_snake_init_pos_reference().x] = SnakeSeg::head_representing_num;
    // delete game_board_objects (no need as it is unique_ptr)
    game_board_objects = std::make_unique<GameBoardObjects>(this);
    game_board_objects->init();

    while (!snake_velocity_queue.empty()) {
        snake_velocity_queue.pop();
    }
    snake_velocity_queue.push(Vector2D(0, 0)); // initial velocity is (0, 0)

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
    if (status != RUNNING) {
        log_and_throw<std::logic_error>("run()", "function should not be called when status != RUNNING");
    }
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                return;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.scancode == SDL_SCANCODE_W || event.key.scancode == SDL_SCANCODE_UP || event.key.key == SDLK_KP_8) {
                    player_direction = Vector2D::get_up_vector();
                } else if (event.key.scancode == SDL_SCANCODE_S || event.key.scancode == SDL_SCANCODE_DOWN || event.key.key == SDLK_KP_5) {
                    player_direction = Vector2D::get_down_vector();
                } else if (event.key.scancode == SDL_SCANCODE_A || event.key.scancode == SDL_SCANCODE_LEFT || event.key.key == SDLK_KP_4) {
                    player_direction = Vector2D::get_left_vector();
                } else if (event.key.scancode == SDL_SCANCODE_D || event.key.scancode == SDL_SCANCODE_RIGHT || event.key.key == SDLK_KP_6) {
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
        update(frame_num, player_direction, std::cout);
        ++frame_num;
        SDL_Delay(MILLIS_PER_FRAME);
    }
}

void Game::add_velocity_to_queue(Vector2D velocity) {
    if (
        velocity.is_opposite_direction_with(snake_velocity_queue.front(), false)
        && game_board_objects->get_snake_length() > 1) {
        snake_velocity_queue.push(snake_velocity_queue.front());
    } else {
        snake_velocity_queue.push(velocity);
    }
}

void Game::update(size_t frame_num, Vector2D next_snake_velocity, std::ostream& os) {
    log("update", "frame_num:" + std::to_string(frame_num), Logger::INFO);
    throw_if_init_not_done("update(size_t frame_num, Vector2D next_snake_velocity, std::ostream& os)");
    if (status == STOP) {
        log("update", "update failed as game have been stopped", Logger::INFO);
        return;
    }
    if (snake_velocity_queue.empty()) {
        log_and_throw<std::logic_error>("update(size_t frame_num, Vector2D next_snake_velocity, std::ostream& os)", "NEVER: snake_velocity_queue is empty, should not happen");
    }
    if (frame_num % snake_period_in_frame_per_square == 0) {
        add_velocity_to_queue(next_snake_velocity);
        move_snake();
        if (status == STOP) {
            return;
        }
    }
    
    Utils::clear_terminal();
    display(
        os, 
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
    init(new_lev_id);
    start_game();
}
void Game::lose() {
    stop_game(LOSED);
    Utils::delay_for_time_in_ms(1000);
    std::string tmp = 
        "level: " + level.get_id() 
        + "\nsnake_length: " + std::to_string(game_board_objects->get_snake_length())
        + "\nstep_no.: " + std::to_string(this->num_of_step)
    ;
    std::cout << "\nYou Losed!\n" + tmp;
    log("lose()", "Game Losed\n" + tmp, Logger::INFO);
}
void Game::win() {
    stop_game(WON);
}


void Game::move_snake(bool force) {
    Vector2D current_direction = snake_velocity_queue.front();
    snake_velocity_queue.pop();
    Vector2D next_velocity = snake_velocity_queue.front();
    log("move_snake(bool force)", "next_velocity:" + next_velocity.to_string(), Logger::INFO);
    throw_if_init_not_done("move_snake");
    if (status == STOP) {
        log("move_snake(bool force)", "move_snake failed as game have been stopped", Logger::INFO);
        return;
    }
    if (current_direction == Vector2D(0, 0)) {
        log("move_snake(bool force)", "move_snake skipped as direction == (0, 0)", Logger::INFO);
        return;
    }
    try {
        if (force) {
            game_board_objects->force_update(next_velocity);
        } else {
            game_board_objects->update(next_velocity);
        }
        ++num_of_step;
    } catch (std::runtime_error e) {
        log_and_throw<Logger::SeeAbove>("move_snake(bool force)", e.what());
    }
    
    
}


// private
void Game::display(std::ostream& os, int n) const {
    log("display(std::ostream& os, int n)", "display", Logger::INFO);
    throw_if_init_not_done("display(std::ostream& os, int n)");
    
    Matrix<char> display_str_matrix(board_size.y, board_size.x*3, ' ');
    int col_num_of_str;
    const std::unique_ptr<Snake> snake = game_board_objects->get_snake_copy();
    const std::vector<Apple>& apples = game_board_objects->get_apples();
    const std::vector<Wall>& walls = game_board_objects->get_walls();
    const std::vector<Pos2D>& empty_poses = game_board_objects->get_empty_poses();
    const auto snake_head_iterator = snake->get_head_iterator();
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
    for (auto it = snake->snake_segments.begin(); it != snake->snake_segments.end(); ++it) {
        auto [c, r] = (*it)->pos.get_as_pair();
        col_num_of_str = c*3+1;
        if (it == snake->get_head_iterator()) {
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
    os << display_str_matrix.join_into_string("\n", "");
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
    log("start_game()", "", Logger::INFO);
    throw_if_init_not_done("start_game");
    if (status == RUNNING) {
        log("start_game()", "game is already running, no need to start", Logger::WARNING_2);
        return;
    }
    status = RUNNING;
    stop_reason = NOT_STOPPING; // reset stop reason
    run();
}
void Game::stop_game(GameStopReason reason) {
    log("stop_game(GameStopReason reason)", "reason:" + game_stop_reason_to_string(reason), Logger::INFO);
    status = STOP;
    stop_reason = reason;
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
            + " SnakeHeadPos:"
        ;
        if (game_board_objects == nullptr || !game_board_objects->init_done) {
            prefix += "/* game_board_objects have not initialized */";
        } else {
            prefix += 
                (game_board_objects->get_snake_copy()->head->pos.to_string(false)) 
                + " snake_length:" 
                + std::to_string(game_board_objects->get_snake_length());
        }
        
    }
    prefix += " ]\n";
    return prefix + StringUtils::add_indent(message, 2);
}