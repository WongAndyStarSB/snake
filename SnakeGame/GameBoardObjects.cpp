#include "GameBoardObjects.hpp"

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

#include "Game.hpp"
// --public:

GameBoardObjects::GameBoardObjects(Game* arg_related_game)
    : related_game(arg_related_game), 
      snake(nullptr), 
      apples(), 
      walls(), 
      empty_poses() {
}


void GameBoardObjects::init() {

    log("init()", 
        "Initializing GameBoardObjects (init_done:" + std::to_string(init_done) + ")", 
        Logger::INFO
    );
    
    if (related_game == nullptr) {
        log_and_throw<std::logic_error>(
            "init(const Level& level)", 
            "related_game is null"
        );
    }
    
    // define locals
    Pos2D snake_init_pos = related_game->level.get_snake_init_pos();
    Matrix<int> board = related_game->level.get_board();
    size_t apple_init_num = related_game->level.get_apple_init_num();


    // update snake in related_board
    related_game->board2d[snake_init_pos.y][snake_init_pos.x]
        = SnakeSeg::head_representing_num;

    // Initialize walls
    walls.reserve(related_game->board_size.y * related_game->board_size.x);
    for (int r = 0; r < board.num_of_row; ++r) {
        for (int c = 0; c < board.num_of_col; ++c) {
            switch (board[r][c]) {
                case Wall::representing_num:
                    walls.emplace_back(Pos2D(c, r));
                    break;
                // case GameBoardObject_Empty::representing_num:
                //     empties.emplace_back(Pos2D(c, r));
                //     break;
            }
        }
    }

    // Initialize empty_poses
    empty_poses.reserve(related_game->board_size.y * related_game->board_size.x - walls.size());
    update_empty_poses();

    // Initialize snake
    snake = std::make_unique<Snake>(snake_init_pos);
    snake->snake_segments.reserve(empty_poses.size()+1); // +1 for head

    // Initialize apples
    apples.reserve(apple_init_num);
    if (apples.size() != apple_init_num) {
        // Initialize apple objects in apples vector
        apples.clear();
        for (int i = 0; i < apple_init_num; ++i) {
            apples.emplace_back();
        }
    }
    for (Apple& apple : apples) {
        apple.randomize_pos(empty_poses);
        related_game->board2d[apple.pos.y][apple.pos.x] = apple.representing_num;
        empty_poses_remove(apple.pos);
    }
    log(
        "init()", 
        "GameBoardObjects initialized with " 
            "1 snake segments, "
            + std::to_string(apples.size()) + " apples, and "
            + std::to_string(walls.size()) + " walls.", 
        Logger::INFO);

    init_done = true;
}

void GameBoardObjects::update(Vector2D next_snake_direction) {
    log(
        "update(Vector2D next_snake_direction)", 
        + ", next_snake_direction:" + next_snake_direction.to_string(), Logger::INFO);
    throw_if_init_not_done("update");
    
    if (snake->snake_segments.size() > 1 && next_snake_direction.is_opposite_direction_with(snake->get_direction(), false)) {
        next_snake_direction = snake->get_direction();
    }
    // Update snake position
    snake_move(next_snake_direction);
    // check collision with apples
    for (Apple &apple : apples) {
        if (snake->head->pos == apple.pos) {
            snake_grow();
            if (!empty_poses.empty()) {
                apple_randomize_pos(apple, true);
            }
            return;
        }
    }
    // check collision with walls
    for (Wall wall : walls) {
        if (snake->head->pos == wall.pos) {
            related_game->lose();
            return;
        }
    }
    // check collision with snake body
    for (auto it = snake->snake_segments.begin(); it < snake->snake_segments.end(); ++it) {
        if ((snake->head->pos == (**it).pos) // head == any_segment
            && (it != snake->snake_segments.begin()+snake->head_index) // segment != head
            ) {
            related_game->win();
            return;
        }
        
    }
    
}

void GameBoardObjects::force_update(const Vector2D& next_snake_direction) {
    log("force_update(const Vector2D& next_snake_direction)", 
        "next_snake_direction: " + next_snake_direction.to_string(), 
        Logger::INFO);
    update(next_snake_direction);
    Matrix<int> tmp_board = related_game->board2d;
    std::vector<Pos2D> tmp_empty_poses = empty_poses;
    update_board();
    update_empty_poses();
    if (tmp_board != related_game->board2d) {
        log("force_update", "update does not update board correctly\n-board from manual update: " + tmp_board.to_string() + "\nboard from objs" + related_game->board2d.to_string(), Logger::WARNING_3);
        log("force_update", "board updated", Logger::INFO);
    }
    if (!Utils::Vector::have_same_elements(tmp_empty_poses, empty_poses)) {
        log("force_update", "update does not update empty_poses correctly\n-empty_poses from manual update: " + Pos2D::vector_to_string(tmp_empty_poses) + "\n-empty_poses from board        : " + Pos2D::vector_to_string(empty_poses), Logger::WARNING_3);
        log("force_update", "empty_poses updated", Logger::INFO);
    }
}


const Snake& GameBoardObjects::get_snake() const {
    throw_if_init_not_done("get_snake() const");
    return snake->get_const_ref();
}
Snake GameBoardObjects::get_snake_copy() const {
    throw_if_init_not_done("get_snake_copy() const");
    return snake->copy();
}
const std::vector<Apple>& GameBoardObjects::get_apples() const {
    return apples;
}
const std::vector<Wall>& GameBoardObjects::get_walls() const {
    return walls;
}
const std::vector<Pos2D>& GameBoardObjects::get_empty_poses() const {
    return empty_poses;
}

size_t GameBoardObjects::get_snake_length() const {
    return snake_length;
}

// --private:

void GameBoardObjects::throw_if_init_not_done(const std::string& method_name, const std::string other_info) const {
    if (!init_done) {
        log_and_throw<std::domain_error>(
            method_name, 
            "ShouldNotBeCalled: the initialization of this has not done" 
                + ((other_info == "")? "" : ("\n" + other_info))
        );
    }
}

// board
/**
 * @brief Updates the game board matrix to reflect the current state of all objects.
 *
 * This function clears the board and redraws all empty_poses, walls, and snake segments.
 * If a temporary board pointer is provided, it swaps it with the current board for the duration
 * of the update, then swaps back at the end. This allows for double-buffering or off-screen updates.
 *
 * @param tmp_board Optional pointer to a temporary Matrix<int> to use for drawing.
 *                  If provided, must have the same dimensions as the current board.
 *                  If nullptr, the current board is updated in place.
 *
 * Throws:
 *   MyException if tmp_board is not nullptr and its size does not match related_board.
 */
void GameBoardObjects::update_board(Matrix<int>* tmp_board) {
    log("update_board(Matrix<int>* tmp_board)", 
        "updating with tmp_board:" 
            + ((tmp_board == nullptr)? "null" : "\n" + StringUtils::add_indent(tmp_board->to_string(), 2)), 
        Logger::INFO);
    
    Matrix<int>& drawing_board = (related_game->board2d);
    // If a temporary board is provided, draw on the temporary board.
    if (tmp_board != nullptr) {
        if (tmp_board->num_of_row != related_game->board_size.y 
            || tmp_board->num_of_col != related_game->board_size.x) {
            log_and_throw<std::domain_error>("update_board(Matrix<int>* tmp_board)", "tmp_board size does not match related_board size");
        }
        drawing_board = (*tmp_board);
    }
    // Clear the board
    for (size_t r = 0; r < drawing_board.num_of_row; ++r) {
        for (size_t c = 0; c < drawing_board.num_of_col; ++c) {
            drawing_board[r][c] = 0;
        }
    }
    // Draw apples and walls
    for (const Apple& apple : apples) {
        drawing_board[apple.pos.y][apple.pos.x] = Apple::representing_num;
    }
    for (const Wall& wall : walls) {
        drawing_board[wall.pos.y][wall.pos.x] = Wall::representing_num;
    }
    // Draw snake body segments
    for (auto it = snake->snake_segments.begin(); it < snake->snake_segments.end(); ++it) {
        drawing_board[(*it)->pos.y][(*it)->pos.x] = SnakeSeg::body_representing_num;
    }
    // Draw snake head (overwrites body if head overlaps a segment)
    drawing_board[snake->head->pos.y][snake->head->pos.x] = SnakeSeg::head_representing_num;

}

// snake
void GameBoardObjects::snake_move(const Vector2D& next_snake_direction) {
    log("snake_move(const Vector2D& next_snake_direction)", 
        "next_snake_direction:"+next_snake_direction.to_string(), 
        Logger::INFO);
    throw_if_init_not_done("snake_move(const Vector2D& next_snake_direction");
    // save old_head temporarily
    SnakeSeg* old_head = snake->head;
    
    // Update snake positions
    try {
        snake->snake_move(next_snake_direction);
    } catch (std::exception e) {
        log_and_throw<Logger::SeeAbove>(
            "snake_move(const Vector2D& next_snake_direction)", 
            e.what()
        );
    }

    //update_empty_poses();
    auto it = empty_poses_find(snake->head->pos);
    if (it == empty_poses.end()) {
        log("snake_move(const Vector2D& next_snake_direction)", 
            "new_head_pos not found in empty_poses (a collision of head with other objs should happen later)", 
            Logger::INFO
        );
    } else {
        (*it) = snake->previous_tail->pos;
    }

    // Update board
    SnakeSeg* new_head = snake->head;
    related_game->board2d[new_head->pos.y][new_head->pos.x] = SnakeSeg::head_representing_num;
    related_game->board2d[old_head->pos.y][old_head->pos.x] = SnakeSeg::body_representing_num;
    related_game->board2d[snake->previous_tail->pos.y][snake->previous_tail->pos.x] = 0;
    
    //std::cout << "head pos: " << snake->head->pos << std::endl;
    //std::cout << "tail pos: " << snake->snake_segments[snake->tail_index]->pos << std::endl;
}

void GameBoardObjects::snake_grow() {
    log("snake_grow()", "function start", Logger::INFO);
    throw_if_init_not_done("snake_grow");
    // Update snake positions
    snake->snake_grow();

    // update_empty_poses();
    empty_poses_remove(snake->snake_segments[snake->tail_index]->pos);
    // update board
    auto& new_tail = *snake->snake_segments[snake->tail_index];
    related_game->board2d[new_tail.pos.y][new_tail.pos.x] = SnakeSeg::body_representing_num;
    // update length
    snake_length++;
}

// apple

// private
void GameBoardObjects::apple_randomize_pos(Apple &apple, bool eaten_by_snake) {
    log("apple_randomize_pos", "apple_original_pos:"+apple.pos.to_string()+" eaten_by_snake:"+std::to_string(eaten_by_snake), Logger::INFO);
    Pos2D tmp = apple.pos;
    // update_apple_pos
    apple.pos = empty_poses[rand() % empty_poses.size()];
    if (tmp == apple.pos) {
        std::string msg = "apple_randomize_pos: LogicErr: Apple's new position(" + apple.pos.to_string() + ") should not be the same as the old position(" + tmp.to_string() + ")!";
        log_and_throw<std::logic_error>("apple_randomize_pos(Apple &apple, bool eaten_by_snake)", msg);
    }

    if (eaten_by_snake) {
        if (empty_poses_find(tmp) != empty_poses.end()) {
            // snake_move did not update empty_poses
            log("apple_randomize_pos", "LogicWarning:snake_move did not update empty_poses", Logger::WARNING_3);
            update_empty_poses(); // force_update
        }
        related_game->board2d[tmp.y][tmp.x] = SnakeSeg::head_representing_num;
    } else {
        // update_empty_poses();
        empty_poses.emplace_back(tmp);
        // update board
        related_game->board2d[tmp.y][tmp.x] = 0;
    }
    // update_empty_poses();
    empty_poses_remove(apple.pos);
    // update board
    related_game->board2d[apple.pos.y][apple.pos.x] = Apple::representing_num;
}

// wall

// empty_poses

// private
void  GameBoardObjects::update_empty_poses(Pos2D* top_left_of_range, Size2D* size_of_range) {
    
    Pos2D default_top_left(0, 0);
    Size2D default_size(related_game->board_size.x, related_game->board_size.y);
    // Initialize local variables and checking arguments
    if (top_left_of_range == nullptr && size_of_range == nullptr) {
        log("update_empty_poses", "top_left_of_range:null, size_of_range:null", Logger::INFO);
        top_left_of_range = &default_top_left;
        size_of_range = &default_size;
        empty_poses.clear();
    } else if (top_left_of_range == nullptr || size_of_range == nullptr) {
        log_and_throw<std::invalid_argument>("update_empty_poses(Pos2D* top_left_of_range, Size2D* size_of_range)", "top_left_of_range and size_of_range should be null tgt or not null tgt");
    } else {
        // both top_left_of_range and size_of_range are not null
        if (top_left_of_range->x < 0 || top_left_of_range->y < 0) {
            log_and_throw<std::invalid_argument>("update_empty_poses(Pos2D* top_left_of_range, Size2D* size_of_range)", "top_left_of_range should be non-negative");
        }
        if (top_left_of_range->x + size_of_range->x > related_game->board_size.x
            || top_left_of_range->y + size_of_range->y > related_game->board_size.y) {
            log_and_throw<std::invalid_argument>("update_empty_poses(Pos2D* top_left_of_range, Size2D* size_of_range)", "top_left_of_range + size_of_range should be in range of related_board");
        }
        related_game->log(
            "update_empty_poses(Pos2D* top_left_of_range, Size2D* size_of_range)", 
            "top_left_of_range:" + top_left_of_range->to_string() 
                + " size_of_range:" + size_of_range->to_string(), 
            Logger::INFO);
    }
    // main logic
    std::vector<Pos2D> poses_to_remove;
    for (int r = top_left_of_range->y; r < top_left_of_range->y+size_of_range->y; ++r) {
        for (int c = top_left_of_range->x; c < top_left_of_range->x+size_of_range->x; ++c) {
            bool found = empty_poses_find(Pos2D(c, r)) != empty_poses.end();
            if (related_game->board2d[r][c] == 0 && !found) {
                empty_poses.emplace_back(Pos2D(c, r));
            } else if (related_game->board2d[r][c] != 0 && found) {
                poses_to_remove.emplace_back(Pos2D(c, r));
            }
        }
    }
    empty_poses_remove(poses_to_remove);
    related_game->log("update_empty_poses(Pos2D* top_left_of_range, Size2D* size_of_range)", "updated", Logger::INFO);
}

// private
std::vector<Pos2D>::iterator GameBoardObjects::empty_poses_find(Pos2D pos) {
    return std::find(empty_poses.begin(), empty_poses.end(), pos);
}

// private
std::vector<Pos2D>::iterator GameBoardObjects::empty_poses_remove(const Pos2D &pos_to_remove) {
    related_game->log(
        "empty_poses_remove(const Pos2D &pos_to_remove)", 
        "pos_to_remove: "+pos_to_remove.to_string(), 
        Logger::INFO);
    auto it = empty_poses.begin();
    for (; it != empty_poses.end(); ++it) {
        if (*it == pos_to_remove) {
            return empty_poses.erase(it);
        }
    }
    return it;
}

// private
std::vector<Pos2D> GameBoardObjects::empty_poses_remove(std::vector<Pos2D> poses_to_remove) {
    log("empty_poses_remove(std::vector<Pos2D> poses_to_remove)", 
        "poses_to_remove: "+Pos2D::vector_to_string(poses_to_remove), 
        Logger::INFO
    );
    
    bool erased;
    for (auto it = empty_poses.begin(); it != empty_poses.end(); ) {
        erased = false;
        for (auto jt = poses_to_remove.begin(); jt != poses_to_remove.end(); ++jt) {
            if (*it == *jt) {
                it = empty_poses.erase(it);
                poses_to_remove.erase(jt);  // after this line, jt is invalidated
                // since we are going to break the inner loop
                // the invalidated jt will not be used anymore
                // so the code is safe for this situation
                // !! be careful when refactoring this code
                erased = true;
                if (poses_to_remove.empty()) {
                    return poses_to_remove;
                }
                break;
            } 
        }
        if (!erased) {
            ++it;
        }
    }
    // If we reach here, it means that 
    // some poses in poses_to_remove were not found in empty_poses
    // So we return the remaining poses_to_remove
    // which were not found in empty_poses
    return poses_to_remove;
}

// logs

void GameBoardObjects::log(const std::string& where, const std::string& message, const Logger::LogLevel& lev) const {
    related_game->log("GameBoardObjects::" + where, message, lev);
}
// --end of file