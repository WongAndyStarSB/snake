#include "Snake.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include "Vector2D.hpp"

Snake::Snake(const Pos2D& init_pos, size_t snake_length)
    : head(nullptr), head_index(0), tail_index(0), previous_tail(nullptr) {
    if (snake_length == 0) {
        Logger::log_and_throw<std::invalid_argument>(
            "Snake::Snake(const Pos2D& init_pos, size_t snake_length)", 
            "invalid_argument: snake_length should be greater than 0"
        );
    }
    // Create the head and add it as the first body part
    this->snake_segments.emplace_back(std::make_unique<SnakeSeg>(init_pos));
    this->head = this->snake_segments[this->head_index].get();
    this->previous_tail = std::make_unique<SnakeSeg>();
    
    if (snake_length > 1) {
        // prevent reallocation of snake_segments
        this->snake_segments.reserve(snake_length);
        // Create the rest of the snake segments
        for (size_t i = 1; i < snake_length; ++i) {
            this->snake_segments.emplace_back(std::make_unique<SnakeSeg>(init_pos));
        }
        // Set the tail index to the last segment
        tail_index = this->snake_segments.size() - 1;
    }
}

Snake Snake::copy() const {
    Snake new_snake(this->head->pos, this->snake_segments.size());
    new_snake.head_index = this->head_index;
    new_snake.tail_index = this->tail_index;
    
    new_snake.previous_tail = std::make_unique<SnakeSeg>(this->previous_tail->pos);
    
    // Change value of new_snake segments to value of this segments
    for (int i = 0; i < this->snake_segments.size(); ++i) {
        (*(new_snake.snake_segments[i])) = (*(this->snake_segments[i]));
    }
    new_snake.head = new_snake.snake_segments[head_index].get();

    return new_snake;
}

const Snake& Snake::get_const_ref() const {
    return *this;
}

Vector2D Snake::get_direction() const {
    return this->head->direction;
}

std::vector<std::unique_ptr<SnakeSeg>>::const_iterator Snake::get_head_iterator() const {
    return snake_segments.begin()+head_index;
}

std::vector<std::unique_ptr<SnakeSeg>>::const_iterator Snake::get_iterator_with_offset(const size_t& offset_from_head) const {
    return snake_segments.begin()+get_segment_index_from_head(offset_from_head);
}


/**
 * @brief Calculates the actual index in the snake_segments container based on an offset from the head.
 *
 * This function computes the index within the snake_segments vector by adding the given
 * offset_from_head to the current head_index and wrapping around using modulo arithmetic.
 * This is useful for accessing snake segments relative to the head, taking into account the
 * circular nature of the underlying storage.
 *
 * @param offset_from_head The number of segments away from the head (0 for head itself).
 * @return The actual index in the snake_segments vector corresponding to the requested segment.
 */
size_t Snake::get_segment_index_from_head(const size_t& offset_from_head) const {
    if (offset_from_head >= snake_segments.size()) {
        Logger::log("Snake::get_segment_index_from_head(const size_t& offset_from_head) const", "InvalidArgument: offset_from_head (value:"+std::to_string(offset_from_head) + ") is out of range (snake_segments.size():" + std::to_string(snake_segments.size()) + ")", Logger::WARNING_1);
    }
    return (head_index + offset_from_head) % snake_segments.size();
}

SnakeSeg* Snake::get_seg_ptr_with_index_from_head(const size_t& offset_from_head) const {
    return snake_segments[get_segment_index_from_head(offset_from_head)].get();
}

void Snake::snake_move(Vector2D next_direction) {
    if (this->snake_segments.empty()) {
        log_and_throw<std::runtime_error>(
            "snake_move(Vector2D next_direction)", 
            "snake_segments should not be empty"
        );
    }
    if (this->tail_index >= this->snake_segments.size()) {
        log_and_throw<std::runtime_error>(
            "snake_move(Vector2D next_direction)",
            "this->tail_index should be less than this->snake_segments.size()"
        );
    }
    //swap unique_ptr stored in snake_segments[tail_index] and previous_tail
    std::swap(this->snake_segments[this->tail_index], this->previous_tail);
    
    // change values stored in snake_segments[tail_index] to values of new head
    // (ptr from the this->previous_tail before, which had useless values)
    // this->head->direction stores the current direction
    this->snake_segments[this->tail_index]->pos = this->head->pos + this->head->direction;
    this->snake_segments[this->tail_index]->direction.change_value_as(next_direction);

    //update head_index and tail_index
    this->head_index = this->tail_index;
    this->tail_index = (this->tail_index + this->snake_segments.size() - 1) % this->snake_segments.size();
    //uptdate ptr stored in head to the ptr of new head
    this->head = this->snake_segments[this->head_index].get();
}

void Snake::snake_grow() {
    tail_index += 1; // it is ok if tail_index == current_size
    snake_segments.insert(snake_segments.begin() + tail_index, std::make_unique<SnakeSeg>(previous_tail->pos));
    
    if (head_index > 0) { // tail_index(after +1) == head_index
        head_index += 1; // no need do (% size) as new_size is already 1 larger
    }
       
}

// SnakeSeg Snake::get_future_head() const {
    
// }