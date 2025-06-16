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


void GameBoardObjects::init(const Level& level) {

    related_game->log("GameBoardObjects::init:\n  Initializing GameBoardObjects with\n  level:" + level.get_id() + " init_done:" + std::to_string(init_done), INFO);
    
    if (related_game == nullptr) {
        Logger::log_and_throw("GameBoardObjects::GameBoardObjects:\n  related_game is null");
    }
    if ((related_game->level.get_id()) != level.get_id()) {
        Logger::log_and_throw("GameBoardObjects::GameBoardObjects:\n  related_game->level.level_num and arg_level.level_num are not the same");
    }
    
    // define locals
    Pos2D snake_init_pos = level.get_snake_init_pos();
    Matrix<int> board = level.get_board();
    size_t apple_init_num = level.get_apple_init_num();


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
    related_game->log(
        "GameBoardObjects::init: GameBoardObjects initialized with " 
            + std::to_string(snake->snake_segments.size()) + " snake segments, "
            + std::to_string(apples.size()) + " apples, and "
            + std::to_string(walls.size()) + " walls.", 
        INFO);

    init_done = true;
}

void GameBoardObjects::update(Vector2D snake_velocity, Vector2D next_snake_velocity) {
    related_game->log(
        "GameBoardObjects::update\n  updating with snake_velocity:" + snake_velocity.to_string()
        + ", next_snake_velocity:" + next_snake_velocity.to_string(), INFO);
    throw_if_init_not_done("update");
    
    if (snake->snake_segments.size() > 1 && snake_velocity.is_opposite_direction_with(snake->get_seg_ptr_with_index_from_head(1)->direction, false)) {
        snake_velocity = snake->get_seg_ptr_with_index_from_head(1)->direction;
    }
    // Update snake position
    snake_move(snake_velocity, next_snake_velocity);
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

void GameBoardObjects::force_update(const Vector2D& snake_velocity, const Vector2D& next_snake_velocity) {
    related_game->log("GameBoardObjects::force_update: snake_velocity: " + snake_velocity.to_string(), INFO);
    update(snake_velocity, next_snake_velocity);
    Matrix<int> tmp_board = related_game->board2d;
    std::vector<Pos2D> tmp_empty_poses = empty_poses;
    update_board();
    update_empty_poses();
    if (tmp_board != related_game->board2d) {
        related_game->log("GameBoardObjects::force_update: LogicErr: GameBoardObjects::update does not update board correctly\n-board from manual update: " + tmp_board.to_string() + "\nboard from objs" + related_game->board2d.to_string(), WARNING_3);
        related_game->log("GameBoardObjects::force_update: board updated", INFO, false);
    }
    if (!Utils::Vector::have_same_elements(tmp_empty_poses, empty_poses)) {
        related_game->log("GameBoardObjects::force_update: LogicErr: GameBoardObjects::update does not update empty_poses correctly\n-empty_poses from manual update: " + Pos2D::vector_to_string(tmp_empty_poses) + "\n-empty_poses from board        : " + Pos2D::vector_to_string(empty_poses), WARNING_3);
        related_game->log("GameBoardObjects::force_update: empty_poses updated", INFO, false);
    }
}



const std::unique_ptr<Snake> GameBoardObjects::get_snake_copy() const {
    throw_if_init_not_done("get_snake");
    return snake->copy();
}
const std::vector<Apple> GameBoardObjects::get_apples_copy() const {
    std::vector<Apple> apples_copy;
    apples_copy.reserve(apples.size());
    for (const Apple apple : apples) {
        apples_copy.emplace_back(apple);
    }
    return apples_copy;
}
const std::vector<Wall> GameBoardObjects::get_walls_copy() const {
    std::vector<Wall> walls_copy;
    walls_copy.reserve(apples.size());
    for (const Wall wall : walls) {
        walls_copy.emplace_back(wall);
    }
    return walls_copy;
}
const std::vector<Pos2D> GameBoardObjects::get_empty_poses_copy() const {
    std::vector<Pos2D> empty_poses_copy;
    empty_poses_copy.reserve(empty_poses.size());
    for (const Pos2D pos : empty_poses) {
        empty_poses_copy.emplace_back(pos);
    }
    return empty_poses_copy;
}

size_t GameBoardObjects::get_snake_length() const {
    return snake_length;
}

// --private:

void GameBoardObjects::throw_if_init_not_done(const std::string& method_name, const std::string other_info) const {
    if (!init_done) {
        Logger::log_and_throw(
            "GameBoardObjects::" + method_name + 
            ": ShouldNotBeCalled: the initialization of this has not done" 
            + ((other_info == "")? "" : "\n"+other_info));
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
    related_game->log("GameBoardObjects::update_board\n  updating with tmp_board:" + ((tmp_board == nullptr)? "null" : "\n"+Utils::String::add_indent(tmp_board->to_string(), 2)), INFO);
    
    Matrix<int>& drawing_board = (related_game->board2d);
    // If a temporary board is provided, draw on the temporary board.
    if (tmp_board != nullptr) {
        if (tmp_board->num_of_row != related_game->board_size.y 
            || tmp_board->num_of_col != related_game->board_size.x) {
            related_game->log_and_throw("GameBoardObjects::update_board:\n  tmp_board size does not match related_board size");
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
void GameBoardObjects::snake_move(const Vector2D& snake_velocity, const Vector2D& next_snake_velocity) {
    related_game->log("GameBoardObjects::snake_move snake_velocity:"+snake_velocity.to_string(), INFO);
    throw_if_init_not_done("snake_move");
    // save old_head temporarily
    SnakeSeg* old_head = snake->head;
    // Update snake positions
    try {
        snake->snake_move(snake_velocity, next_snake_velocity);
    } catch (std::runtime_error e) {
        related_game->log_and_throw(std::string(e.what()));
    }

    //update_empty_poses();
    auto it = empty_poses_find(snake->head->pos);
    if (it == empty_poses.end()) {
        related_game->log("GameBoardObjects::snake_move: new_head_pos not found in empty_poses (a collision of head with other objs should happen later)", INFO);
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
    related_game->log("GameBoardObjects::snake_grow", INFO);
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
void GameBoardObjects::apple_randomize_pos(Apple &apple, bool eaten_by_snake) {
    related_game->log("GameBoardObjects::apple_randomize_pos apple_original_pos:"+apple.pos.to_string()+" eaten_by_snake:"+std::to_string(eaten_by_snake), INFO);
    Pos2D tmp = apple.pos;
    // update_apple_pos
    apple.pos = empty_poses[rand() % empty_poses.size()];
    if (tmp == apple.pos) {
        std::string msg = "GameBoardObjects::apple_randomize_pos: LogicErr: Apple's new position(" + apple.pos.to_string() + ") should not be the same as the old position(" + tmp.to_string() + ")!";
        related_game->log_and_throw(msg);
    }

    if (eaten_by_snake) {
        if (empty_poses_find(tmp) != empty_poses.end()) {
            // snake_move did not update empty_poses
            related_game->log("GameBoardObjects::apple_randomize_pos: LogicErr: snake_move did not update empty_poses", WARNING_3);
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
void  GameBoardObjects::update_empty_poses(Pos2D* top_left_of_range, Size2D* size_of_range) {
    
    Pos2D default_top_left(0, 0);
    Size2D default_size(related_game->board_size.x, related_game->board_size.y);
    // Initialize local variables and checking arguments
    if (top_left_of_range == nullptr && size_of_range == nullptr) {
        related_game->log("GameBoardObjects::update_empty_poses\n  top_left_of_range:null, size_of_range:null", INFO);
        top_left_of_range = &default_top_left;
        size_of_range = &default_size;
        empty_poses.clear();
    } else if (top_left_of_range == nullptr || size_of_range == nullptr) {
        std::string msg = "GameBoardObjects::update_empty_poses: InvalidArgument: top_left_of_range and size_of_range should be null tgt or not null tgt";
        related_game->log_and_throw(msg);
    } else {
        // both top_left_of_range and size_of_range are not null
        if (top_left_of_range->x < 0 || top_left_of_range->y < 0) {
            std::string msg = "GameBoardObjects::update_empty_poses: InvalidArgument: top_left_of_range should be non-negative";
            related_game->log_and_throw(msg);
        }
        if (top_left_of_range->x + size_of_range->x > related_game->board_size.x
            || top_left_of_range->y + size_of_range->y > related_game->board_size.y) {
            related_game->log_and_throw("GameBoardObjects::update_empty_poses: InvalidArgument: top_left_of_range + size_of_range should be in range of related_board");
        }
        related_game->log("GameBoardObjects::update_empty_poses\n  top_left_of_range:"+top_left_of_range->to_string()+" size_of_range:"+size_of_range->to_string(), INFO);
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
    related_game->log("GameBoardObjects::update_empty_poses updated", INFO);
}

std::vector<Pos2D>::iterator GameBoardObjects::empty_poses_find(Pos2D pos) {
    return std::find(empty_poses.begin(), empty_poses.end(), pos);
}

std::vector<Pos2D>::iterator GameBoardObjects::empty_poses_remove(const Pos2D &pos_to_remove) {
    related_game->log("GameBoardObjects::empty_poses_remove pos_to_remove: "+pos_to_remove.to_string(), INFO);
    auto it = empty_poses.begin();
    for (; it != empty_poses.end(); ++it) {
        if (*it == pos_to_remove) {
            return empty_poses.erase(it);
        }
    }
    return it;
}

std::vector<Pos2D> GameBoardObjects::empty_poses_remove(std::vector<Pos2D> poses_to_remove) {
    related_game->log("GameBoardObjects::empty_poses_remove\n  poses_to_remove: "+Pos2D::vector_to_string(poses_to_remove), INFO);
    
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

// --end of file