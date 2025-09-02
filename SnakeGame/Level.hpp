#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <unordered_map>
#include <memory>

#include "../Logger/Logger.hpp"
#include "Matrix.hpp"
#include "Pos2D.hpp"

class Level { //dataclass
  private:
    std::string id;
    Matrix<int> board;
    Pos2D snake_init_pos;
    size_t apple_init_num;
    bool changeable;

    static std::unordered_map<std::string, std::unique_ptr<Level>> existing_levels;

    static const std::string ID_FORMAT;
    static const std::string ORIG_PREFIX;
    static const std::string COPY_PREFIX;
    static const uint8_t ID_LENGTH = 17;

  public:
    
    
    

    // factory function
    static const Level* create_and_register(
        const std::string& arg_id, 
        const Matrix<int>& arg_board, 
        const Pos2D& arg_snake_init_pos, 
        const size_t& arg_apple_init_num
    );
    
    // constructor
    explicit Level(
        const std::string& arg_id, 
        const Matrix<int>& arg_board, 
        const Pos2D& arg_snake_init_pos, 
        const size_t& arg_apple_init_num, 
        const bool& arg_variable
    );
    // copy constructor
    Level(const Level& level);
    // move constructor
    inline Level(Level&&) = default;

    Level& operator=(const Level&);
    Level& operator=(Level&&) = default;

    Level get_copy() const;

    std::string get_id() const;
    Matrix<int> get_board() const;
    Pos2D get_snake_init_pos() const;
    size_t get_apple_init_num() const;
    bool get_changeable() const;

    Matrix<int>& get_board_reference();
    Pos2D& get_snake_init_pos_reference();
    

    void set_id(std::string new_id);
    void set_board(Matrix<int> new_board);
    void set_snake_init_pos(Pos2D new_init_pos);
    void set_apple_init_num(size_t new_init_num);


    void switch_to_unchangeable();


    static const Level& find_level(const std::string& id);



    
    inline ~Level() {
        Logger::log(
          "Level::~Level() /*destructor*/", 
          "Level("+id+") destroyed", 
          Logger::INFO);
    }

  private:
    
    inline void log(const std::string& where, const std::string& message, const Logger::LogLevel& lev) {
      Logger::log("Level object (id: " + id + ")\nLevel::" + where, message, lev, true);
    }

    template <typename ExceptionType>
    [[noreturn]] inline void log_and_throw(const std::string& where, const std::string& message) {
      Logger::log_and_throw<ExceptionType>("Level object (id: " + id + ")::" + where, message);
    }

    bool is_id_exist_in_map(const std::string& arg_id);
    bool is_id_len_suits(const std::string& arg_id);
    
    
};

#endif