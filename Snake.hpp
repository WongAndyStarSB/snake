#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <memory>
#include <vector>

#include "Logger.hpp"
#include "GameBoardObject.hpp"

class Snake {
public:
    SnakeSeg* head;
    std::vector<std::unique_ptr<SnakeSeg>> snake_segments;
    size_t head_index;
    size_t tail_index;
    std::unique_ptr<SnakeSeg> previous_tail;

    explicit Snake(const Pos2D& init_pos, size_t snake_length = 1);
    // copy constructor
    Snake(const Snake&) = delete; // disable copy constructor
    Snake& operator=(const Snake&) = delete; // disable copy assignment
    Snake(Snake&&) = default; // enable move constructor

    std::unique_ptr<Snake> copy() const;
    std::vector<std::unique_ptr<SnakeSeg>>::const_iterator get_head_iterator() const;
    std::vector<std::unique_ptr<SnakeSeg>>::const_iterator get_iterator_with_offset(const size_t& offset_from_head) const;
    size_t get_segment_index_from_head(const size_t& offset_from_head) const;
    SnakeSeg* get_seg_ptr_with_index_from_head(const size_t& offset_from_head) const;

    void snake_move(Vector2D direction, Vector2D next_direction);
    void snake_grow();
};

#endif // SNAKE_HPP