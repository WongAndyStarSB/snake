#ifndef LEVELS
#define LEVELS

#include <unordered_map>
#include <memory>

#include "Logger.hpp"

#include "Matrix.hpp"
#include "Pos2D.hpp"
#include "Level.hpp"

namespace snake{
namespace levels{


inline const void init_testing_levels() {
    Logger::log("Initializing testing levels...", INFO);

    
    const Matrix<int> TEST_1_BOARD( {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1},
    });
    const Pos2D TEST_1_SNAKE_INIT_POS(1, 1);
    const int TEST_1_APPLE_INIT_NUM = 4;
    Level::create_and_register("T001", TEST_1_BOARD, TEST_1_SNAKE_INIT_POS, TEST_1_APPLE_INIT_NUM);

    Logger::log("testing levels initialized", INFO);
}

inline const void init_levels() {
    Logger::log("Initializing levels...", INFO);

    
    
    const Matrix<int> SAMPLE_BOARD( {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    });

    const Matrix<int> LEVEL_0_BOARD( {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    });

    
    const Matrix<int> LEVEL_1_BOARD( {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    });
    const Matrix<int> LEVEL_2_BOARD( {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1}, 
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    });
    const Matrix<int> YP01_BOARD( {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    });
    

    const Pos2D LEVEL_0_SNAKE_INIT_POS(3, 3);
    const Pos2D LEVEL_1_SNAKE_INIT_POS(4, 3);
    const Pos2D LEVEL_2_SNAKE_INIT_POS(2, 3);
    const Pos2D YP01_SNAKE_INIT_POS(8, 1);

    
    const int LEVEL_0_APPLE_INIT_NUM = 1;
    const int LEVEL_1_APPLE_INIT_NUM = 3;
    const int LEVEL_2_APPLE_INIT_NUM = 3;
    const int YP01_APPLE_INIT_NUM = 3;

    
    Level::create_and_register("SAMP", SAMPLE_BOARD, Pos2D(0, 0), 0); // Sample level
    Level::create_and_register("0000", LEVEL_0_BOARD, LEVEL_0_SNAKE_INIT_POS, LEVEL_0_APPLE_INIT_NUM);
    Level::create_and_register("0001", LEVEL_1_BOARD, LEVEL_1_SNAKE_INIT_POS, LEVEL_1_APPLE_INIT_NUM);
    Level::create_and_register("0002", LEVEL_2_BOARD, LEVEL_2_SNAKE_INIT_POS, LEVEL_2_APPLE_INIT_NUM);
    Level::create_and_register("YP01", YP01_BOARD, YP01_SNAKE_INIT_POS, YP01_APPLE_INIT_NUM);
    Logger::log("Levels initialized", INFO);
}

}
}
#endif