#include "Level.hpp"
#include "Logger.hpp"

std::unordered_map<std::string, std::unique_ptr<Level>> Level::existing_levels{};

Level::Level(const std::string& arg_id, const Matrix<int>& arg_board, const Pos2D& arg_snake_init_pos, const size_t& arg_apple_init_num, const bool& arg_changeable)
    : id(arg_id), board(arg_board), snake_init_pos(arg_snake_init_pos), apple_init_num(arg_apple_init_num), changeable(arg_changeable) {
        Logger::log("Level("+id+") created", INFO);
    }
Level::Level(const Level& level) 
    : 
    id(level.get_id()), 
    board(level.get_board()), 
    snake_init_pos(level.get_snake_init_pos()), 
    apple_init_num(level.get_apple_init_num()),
    changeable(true) 
{}

const Level* Level::create_and_register(const std::string& arg_id,
    const Matrix<int>& arg_board,
    const Pos2D& arg_snake_init_pos,
    const size_t& arg_apple_init_num) {
    Logger::log("Level::create_and_register: Attempting to create and register level with id: " + arg_id, INFO);
    if (existing_levels.find(arg_id) != existing_levels.end()) {
        Logger::log_and_throw("Level::create_and_register InvalidArgument: level with id " + arg_id + " already exists");
    }
    auto [it, inserted] = existing_levels.emplace(
        arg_id,
        std::make_unique<Level>(arg_id, arg_board, arg_snake_init_pos, arg_apple_init_num, false)
    );
    return (it->second).get();
}




// getters 

Level Level::get_copy() const {
    return Level(id, board, snake_init_pos, apple_init_num, true);
}

std::string Level::get_id() const {
    return id;
}
Matrix<int> Level::get_board() const {
    return board;
}
Pos2D Level::get_snake_init_pos() const {
    return snake_init_pos;
}
size_t Level::get_apple_init_num() const {
    return apple_init_num;
}
bool Level::get_changeable() const {
    return changeable;
}

Matrix<int>& Level::get_board_reference() {
    if (!changeable) {
        Logger::log_and_throw("Level::get_board_reference try to get member reference of non-changeable level");
    }
    return board;
}
Pos2D Level::get_snake_init_pos_reference() {
    if (!changeable) {
        Logger::log_and_throw("Level::get_snake_init_pos_reference try to get member reference of non-changeable level");
    }
    return snake_init_pos;
}

// setters
void Level::set_id(std::string new_id) {
    if (!changeable) {
        Logger::log_and_throw("Level::set_id try to change non-changeable level");
    }
    id = new_id;
}
void Level::set_board(Matrix<int> new_board) {
    if (!changeable) {
        Logger::log_and_throw("Level::set_board try to change non-changeable level");
    }
    board = new_board;
}
void Level::set_snake_init_pos(Pos2D new_init_pos) {
    if (!changeable) {
        Logger::log_and_throw("Level::set_snake_init_pos try to change non-changeable level");
    }
    snake_init_pos = new_init_pos;
}
void Level::set_apple_init_num(size_t new_init_num) {
    if (!changeable) {
        Logger::log_and_throw("Level::set_apple_init_num try to change non-changeable level");
    }
    apple_init_num = new_init_num;
}

void Level::change_value_as(const Level& other) {
    if (this == &other) return;
    if (!changeable) {
        Logger::log_and_throw("Level::change_value_as try to change non-changeable level");
    }

    id = other.get_id();
    board = other.get_board();
    snake_init_pos = other.get_snake_init_pos();
    apple_init_num = other.get_apple_init_num();
    changeable = other.get_changeable();
}

void Level::switch_to_unchangeable() {
    changeable = false;
}

const Level& Level::find_level(const std::string& id) {
    auto it = existing_levels.find(id);
    if (it == existing_levels.end()) {
        Logger::log_and_throw("Level::find_level InvalidArgument: level not found");
    } else {
        return *(it->second);
    }
}