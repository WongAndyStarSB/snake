cmake_minimum_required(VERSION 3.10.0)
project(snake VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 設置編譯器選項
if (MSVC)
    # Windows
    add_compile_options(/W4)
else()
    # macOS
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()





# set compile directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

# set output exe file name
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 查找並載入Cmake預設
# include_directories("E:/Library/SDL3/include") # no need
# link_directories("E:/Library/SDL3/lib/x64") # no need
find_package(SDL3 REQUIRED)
if (NOT SDL3_FOUND)
    message(FATAL_ERROR "SDL3 not found. Please install SDL3.")
endif()

# 
# set(CMAKE_VERBOSE_MAKEFILE ON)
# add_compile_options(/Bt /d2cgsummary)

# 添加可執行文件
# add_executable(${TARGET}
#     testing.cpp
# )
add_executable(
    ${TARGET}
    ../Math/Fraction.cpp
)

# Get all source files in a specific folder (e.g., 'SnakeGame') using a variable

file(GLOB UTILS_SOURCES "Utils/*.cpp")
file(GLOB LOGGER_SOURCES "Logger/*.cpp")
file(GLOB MATH_SOURCES "Math/*.cpp")
file(GLOB APP_SOURCES "App/*.cpp")
file(GLOB SNAKE_GAME_SOURCES "SnakeGame/*.cpp")

# Add the source files to the executable target
target_sources(${TARGET} PRIVATE 
    ${UTILS_SOURCES}
    ${LOGGER_SOURCES}
    ${MATH_SOURCES}
    # ${APP_SOURCES}
    ${SNAKE_GAME_SOURCES}
)

# 鏈接SDL3庫
target_link_libraries(${TARGET}
                        ${SDL3_LIBRARIES})