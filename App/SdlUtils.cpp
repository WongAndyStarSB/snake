#include "SdlUtils.hpp"

#include <string>
#include <stdexcept>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include "Logger.hpp"

namespace { // Anonymous namespace for private functions

template <typename ExceptionType>
[[noreturn]] inline void log_and_throw(const std::string& where, const std::string& message) {
    Logger::log_and_throw<ExceptionType>("SdlUtils::" + where, message);
}
template <typename ExceptionType = std::runtime_error>
[[noreturn]] inline void log_and_throw_SDL_failure(const std::string& where, const std::string& sdl_func_name) {
    Logger::log_and_throw<ExceptionType>(
        "SdlUtils::" + where, 
        sdl_func_name + " failed: " + SDL_GetError()
    );
}


// private function to recursively fill the area with the target color using SDL_Color
// caution: it does not check the renderer
void helper_flood_fill_target_color(::SDL_Renderer* renderer, const ::SDL_Point& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color) {
    ::SDL_Color color; // render_draw_color at starting_pos
    if (! ::SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a)) {
        log_and_throw_SDL_failure(
            "flood_fill_target_color(const ::SDL_Point& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            "SDL_GetRenderDrawColor"
        );
    }
    if (!(color.r == target_color.r 
        && color.g == target_color.g 
        && color.b == target_color.b 
        && color.a == target_color.a)) { // (color != target_color)
        return; // not target of fill
    }
    if (! ::SDL_SetRenderDrawColor(renderer, result_color.r, result_color.g, result_color.b, result_color.a)) {
        log_and_throw_SDL_failure(
            "flood_fill_target_color(const ::SDL_Point& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            "SDL_SetRenderDrawColor"
        );
    }
    if (! ::SDL_RenderPoint(renderer, starting_pos.x, starting_pos.y)) {
        log_and_throw_SDL_failure(
            "flood_fill_target_color(const ::SDL_Point& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            "SDL_RenderPoint"
        );
    }
    
    ::SDL_Point points[4] = {
        {starting_pos.x + 1, starting_pos.y},
        {starting_pos.x - 1, starting_pos.y},
        {starting_pos.x, starting_pos.y + 1},
        {starting_pos.x, starting_pos.y - 1}
    };

    for (int i = 0; i < 4; ++i) {
        helper_flood_fill_target_color(renderer, points[i], target_color, result_color);
    }
}

// private function to recursively fill the area with the target color using ARGB format
// caution: it does not check the renderer
void helper_flood_fill_target_color(::SDL_Renderer* renderer, int x, int y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB) {
    uint32_t color_ARGB;
    if (! ::SDL_GetRenderDrawColor(
        renderer, 
        reinterpret_cast<uint8_t*>(&color_ARGB) + 1, 
        reinterpret_cast<uint8_t*>(&color_ARGB) + 2, 
        reinterpret_cast<uint8_t*>(&color_ARGB) + 3, 
        reinterpret_cast<uint8_t*>(&color_ARGB)
    )) {
        log_and_throw_SDL_failure(
            "flood_fill_target_color(int x, int y, uint32_t target_color_ARGB, uint32_t result_color_ARGB)", 
            "SDL_GetRenderDrawColor"
        );
    }
    if (color_ARGB != target_color_ARGB) {
        return; // not target of fill
    }
    if (! ::SDL_SetRenderDrawColor(
        renderer, 
        (result_color_ARGB >> 16) & 0xFF, 
        (result_color_ARGB >> 8) & 0xFF, 
        (result_color_ARGB) & 0xFF, 
        (result_color_ARGB >> 24) & 0xFF
    )) {
        log_and_throw_SDL_failure(
            "flood_fill_target_color(int x, int y, uint32_t target_color_ARGB, uint32_t result_color_ARGB)", 
            "SDL_SetRenderDrawColor"
        );
    }
    if (! ::SDL_RenderPoint(renderer, x, y)) {
        log_and_throw_SDL_failure(
            "flood_fill_target_color(int x, int y, uint32_t target_color_ARGB, uint32_t result_color_ARGB)", 
            "SDL_RenderPoint"
        );
    }
    int points[4][2] = {
        {x + 1, y},
        {x - 1, y},
        {x, y + 1},
        {x, y - 1}
    };
    for (int i = 0; i < 4; ++i) {
        helper_flood_fill_target_color(renderer, points[i][0], points[i][1], target_color_ARGB, result_color_ARGB);
    }
}

// private function to recursively fill the area with the target color using SDL_Color
// This is a specialized version for SDL_FPoint, which uses float coordinates
// caution: it does not check the renderer
void helper_flood_fill_target_color_F(::SDL_Renderer* renderer, const ::SDL_FPoint& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color) {
    ::SDL_Color color; // render_draw_color at starting_pos
    if (! ::SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a)) {
        log_and_throw_SDL_failure(
            "flood_fill_target_color(const ::SDL_FPoint& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            "SDL_GetRenderDrawColor"
        );
    }
    if (!(color.r == target_color.r 
        && color.g == target_color.g 
        && color.b == target_color.b 
        && color.a == target_color.a)) { // (color != target_color)
        return; // not target of fill
    }
    if (! ::SDL_SetRenderDrawColor(renderer, result_color.r, result_color.g, result_color.b, result_color.a)) {
        log_and_throw_SDL_failure(
            "flood_fill_target_color(const ::SDL_FPoint& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            "SDL_SetRenderDrawColor"
        );
    }
    if (! ::SDL_RenderPoint(renderer, starting_pos.x, starting_pos.y)) {
        log_and_throw_SDL_failure(
            "flood_fill_target_color(const ::SDL_FPoint& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            "SDL_RenderPoint"
        );
    }
    
    ::SDL_FPoint points[4] = {
        {starting_pos.x + 1, starting_pos.y},
        {starting_pos.x - 1, starting_pos.y},
        {starting_pos.x, starting_pos.y + 1},
        {starting_pos.x, starting_pos.y - 1}
    };

    for (int i = 0; i < 4; ++i) {
        helper_flood_fill_target_color_F(renderer, points[i], target_color, result_color);
    }
}

// private function to recursively fill the area with the target color using ARGB format
// This is a specialized version for SDL_FPoint, which uses float coordinates
// caution: it does not check the renderer
void helper_flood_fill_target_color_F(::SDL_Renderer* renderer, float x, float y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB) {
    uint32_t color_ARGB;
    if (! ::SDL_GetRenderDrawColor(
        renderer, 
        reinterpret_cast<uint8_t*>(&color_ARGB) + 1, 
        reinterpret_cast<uint8_t*>(&color_ARGB) + 2, 
        reinterpret_cast<uint8_t*>(&color_ARGB) + 3, 
        reinterpret_cast<uint8_t*>(&color_ARGB)
    )) {
        log_and_throw_SDL_failure(
            "helper_flood_fill_target_color_F(::SDL_Renderer* renderer, float x, float y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB)", 
            "SDL_GetRenderDrawColor"
        );
    }
    if (color_ARGB != target_color_ARGB) {
        return; // not target of fill
    }
    if (! ::SDL_SetRenderDrawColor(
        renderer, 
        (result_color_ARGB >> 16) & 0xFF, 
        (result_color_ARGB >> 8) & 0xFF, 
        (result_color_ARGB) & 0xFF, 
        (result_color_ARGB >> 24) & 0xFF
    )) {
        log_and_throw_SDL_failure(
            "helper_flood_fill_target_color_F(::SDL_Renderer* renderer, float x, float y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB)", 
            "SDL_SetRenderDrawColor"
        );
    }
    if (! ::SDL_RenderPoint(renderer, x, y)) {
        log_and_throw_SDL_failure(
            "helper_flood_fill_target_color_F(::SDL_Renderer* renderer, float x, float y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB)", 
            "SDL_RenderPoint"
        );
    }
    float points[4][2] = {
        {x + 1, y},
        {x - 1, y},
        {x, y + 1},
        {x, y - 1}
    };
    for (int i = 0; i < 4; ++i) {
        helper_flood_fill_target_color_F(renderer, points[i][0], points[i][1], target_color_ARGB, result_color_ARGB);
    }
}

} // Anonymous namespace end

void SdlUtils::flood_fill(::SDL_Renderer* renderer, const ::SDL_Point& starting_pos, const ::SDL_Color& result_color) {
    if (!renderer) {
        log_and_throw<std::logic_error>(
            "flood_fill(const ::SDL_Point& starting_pos, const ::SDL_Color& result_color)", 
            "Renderer is null");
    }
    
    ::SDL_Color color;
    if (! ::SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a)) {
        log_and_throw_SDL_failure(
            "flood_fill(const ::SDL_Point& starting_pos, const ::SDL_Color& result_color)", 
            "SDL_GetRenderDrawColor"
        );
    }

    try {
        // call the private one to avoid checking the renderer again
        helper_flood_fill_target_color(renderer, starting_pos, color, result_color);
    } catch (const std::exception& e) {
        log_and_throw<Logger::SeeAbove>(
            "flood_fill(const ::SDL_Point& starting_pos, const ::SDL_Color& result_color)", 
            e.what()
        );
    }
}

void SdlUtils::flood_fill(::SDL_Renderer* renderer, int x, int y, const uint32_t& result_color_ARGB) {
    if (!renderer) {
        log_and_throw<std::logic_error>(
            "flood_fill(int x, int y, uint32_t result_color_ARGB)", 
            "Renderer is null"
        );
    }
    // Get the current render draw color
    ::uint32_t color_ARGB;
    if (! ::SDL_GetRenderDrawColor(
        renderer, 
        reinterpret_cast<Uint8*>(color_ARGB) + 1,
        reinterpret_cast<Uint8*>(color_ARGB) + 2,
        reinterpret_cast<Uint8*>(color_ARGB) + 3,
        reinterpret_cast<Uint8*>(color_ARGB)
    )) {
        log_and_throw_SDL_failure(
            "flood_fill(int x, int y, uint32_t result_color_ARGB)", 
            "SDL_GetRenderDrawColor"
        );
    }
    try {
        // call the private one to avoid checking the renderer again
        helper_flood_fill_target_color(renderer, x, y, color_ARGB, result_color_ARGB);
    } catch (const std::exception& e) {
        log_and_throw<Logger::SeeAbove>(
            "flood_fill(int x, int y, uint32_t result_color_ARGB)", 
            e.what()
        );
    }
    
}

void SdlUtils::flood_fill_F(::SDL_Renderer* renderer, const ::SDL_FPoint& starting_pos, const ::SDL_Color& result_color) {
    if (!renderer) {
        log_and_throw<std::logic_error>(
            "flood_fill(const ::SDL_Point& starting_pos, const ::SDL_Color& result_color)", 
            "Renderer is null");
    }
    
    ::SDL_Color color;
    if (! ::SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a)) {
        log_and_throw_SDL_failure(
            "flood_fill(const ::SDL_Point& starting_pos, const ::SDL_Color& result_color)", 
            "SDL_GetRenderDrawColor"
        );
    }
    try {
        // call the private one to avoid checking the renderer again
        helper_flood_fill_target_color_F(renderer, starting_pos, color, result_color);
    } catch (const std::exception& e) {
        log_and_throw<Logger::SeeAbove>(
            "flood_fill_F(const ::SDL_FPoint& starting_pos, const ::SDL_Color& result_color)", 
            e.what()
        );
    }
}

void SdlUtils::flood_fill_F(::SDL_Renderer* renderer, float x, float y, const uint32_t& result_color_ARGB) {
    if (!renderer) {
        log_and_throw<std::logic_error>(
            "flood_fill_F(float x, float y, const uint32_t& result_color_ARGB)", 
            "Renderer is null"
        );
    }
    
    // Get the current render draw color
    ::uint32_t color_ARGB;
    if (! ::SDL_GetRenderDrawColor(
        renderer, 
        reinterpret_cast<Uint8*>(&color_ARGB) + 1,
        reinterpret_cast<Uint8*>(&color_ARGB) + 2,
        reinterpret_cast<Uint8*>(&color_ARGB) + 3,
        reinterpret_cast<Uint8*>(&color_ARGB)
    )) {
        log_and_throw_SDL_failure(
            "flood_fill_F(float x, float y, const uint32_t& result_color_ARGB)", 
            "SDL_GetRenderDrawColor"
        );
    }
    try {
        // call the private one to avoid checking the renderer again
        helper_flood_fill_target_color_F(renderer, x, y, color_ARGB, result_color_ARGB);
    } catch (const std::exception& e) {
        log_and_throw<Logger::SeeAbove>(
            "flood_fill_F(float x, float y, const uint32_t& result_color_ARGB)", 
            e.what()
        );
    }
}

void SdlUtils::flood_fill_target_color(::SDL_Renderer* renderer, const ::SDL_Point& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color) {
    if (!renderer) {
        log_and_throw<std::logic_error>(
            "flood_fill_target_color(const ::SDL_Point& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            "Renderer is null"
        );
    }
    try {
        helper_flood_fill_target_color(renderer, starting_pos, target_color, result_color);
    } catch (const std::exception& e) {
        log_and_throw<Logger::SeeAbove>(
            "flood_fill_target_color(const ::SDL_Point& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            e.what()
        );
    }
}

void SdlUtils::flood_fill_target_color(::SDL_Renderer* renderer, int x, int y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB) {
    if (!renderer) {
        log_and_throw<std::logic_error>(
            "flood_fill_target_color(int x, int y, uint32_t target_color_ARGB, uint32_t result_color_ARGB)", 
            "Renderer is null"
        );
    }
    try {
        helper_flood_fill_target_color(renderer, x, y, target_color_ARGB, result_color_ARGB);
    } catch (const std::exception& e) {
        log_and_throw<Logger::SeeAbove>(
            "flood_fill_target_color(int x, int y, uint32_t target_color_ARGB, uint32_t result_color_ARGB)", 
            e.what()
        );
    }
}

void SdlUtils::flood_fill_target_color_F(::SDL_Renderer* renderer, const ::SDL_FPoint& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color) {
    if (!renderer) {
        log_and_throw<std::logic_error>(
            "flood_fill_target_color_F(const ::SDL_FPoint& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            "Renderer is null"
        );
    }
    try {
        helper_flood_fill_target_color_F(renderer, starting_pos, target_color, result_color);
    } catch (const std::exception& e) {
        log_and_throw<Logger::SeeAbove>(
            "flood_fill_target_color_F(const ::SDL_FPoint& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color)", 
            e.what()
        );
    }
}

void SdlUtils::flood_fill_target_color_F(::SDL_Renderer* renderer, float x, float y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB) {
    if (!renderer) {
        log_and_throw<std::logic_error>(
            "flood_fill_target_color(float x, float y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB)", 
            "Renderer is null"
        );
    }
    try {
        helper_flood_fill_target_color_F(renderer, x, y, target_color_ARGB, result_color_ARGB);
    } catch (const std::exception& e) {
        log_and_throw<Logger::SeeAbove>(
            "flood_fill_target_color(float x, float y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB)", 
            e.what()
        );
    }
}