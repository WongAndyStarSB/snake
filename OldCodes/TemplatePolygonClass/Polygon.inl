#if 0

#ifndef POLYGON_INL
#define POLYGON_INL

#include "Polygon.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

namespace snake {

// Implement member functions for the Polygon class

template <size_t NumOfSides>
inline void Polygon<NumOfSides>::set_centre_pos(const Pos2D& new_centre_pos) {
    centre_pos = new_centre_pos;
    // Update the positions of the vertices based on the new centre position
    for (size_t i = 0; i < NumOfSides; ++i) {
        pos_of_vertices[i] = {vertices[i].x + centre_pos.x, vertices[i].y + centre_pos.y};
    }
}

template <size_t NumOfSides>
inline void Polygon<NumOfSides>::draw(bool arg_fill) const {
    try {
        draw(centre_pos, outline_color, fill_color, arg_fill);
    } catch (const std::exception& e) {
        log_and_throw<Logger::SeeAbove>("draw(bool fill = true)", e.what());
    }
}

template <size_t NumOfSides>
inline void Polygon<NumOfSides>::draw(bool arg_fill, const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const {
    if (!renderer) {
        log_and_throw<std::logic_error>("draw(bool fill = true, const ::SDL_Color& arg_outline_color = outline_color, const ::SDL_Color& arg_fill_color = fill_color) const", "Renderer is null");
    }
    if (arg_fill) {
        draw_with_fill(arg_outline_color, arg_fill_color);
    } else {
        draw_outline(arg_outline_color);
    }
}

// private member functions for the Polygon class

template <size_t NumOfSides>
inline void Polygon<NumOfSides>::draw_outline(const ::SDL_Color& arg_color) const {
    if (! ::SDL_SetRenderDrawColor(renderer, arg_color.r, arg_color.g, arg_color.b, arg_color.a)) {
        log_and_throw<std::runtime_error>(
            "draw_outline(const ::SDL_Color& arg_color) const", 
            "SDL_SetRenderDrawColor failed: " + SDL_GetError()
        );
    }
    for (const auto& vertex : vertices) {
        sdl_points.emplace_back({vertex.x + arg_centre_pos.x, vertex.y + arg_centre_pos.y});
    }
    if (! ::SDL_RenderLines(renderer, sdl_points.data(), sdl_points.size())) {
        log_and_throw<std::runtime_error>(
            "draw_outline(const ::SDL_Color& arg_color) const", 
            "SDL_RenderLines failed: " + SDL_GetError()
        );
    }
}

template <size_t NumOfSides>
inline void Polygon<NumOfSides>::draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const {
    if (! ::SDL_SetRenderDrawColor(renderer, arg_fill_color.r, arg_fill_color.g, arg_fill_color.b, arg_fill_color.a)) {
        log_and_throw_SDL_failure<std::runtime_error>(
            "fill(const ::SDL_Color& arg_fill_color)", 
            "SDL_SetRenderDrawColor failed: " + SDL_GetError()
        );
    }
    // Create an off-screen buffer to draw the polygon
    int windowWidth, windowHeight;
    if (! ::SDL_GetWindowSize(window, &windowWidth, &windowHeight)) {
        log_and_throw_SDL_failure<std::runtime_error>(
            "draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_GetWindowSize failed: " + SDL_GetError()
        );
    }
    
    ::SDL_Surface* offscreenSurface = ::SDL_CreateSurface(windowWidth, windowHeight, ::SDL_PIXELFORMAT_RGBA32);
    if (!offscreenSurface) {
        log_and_throw_SDL_failure<std::runtime_error>(
            "draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_CreateSurface failed: " + SDL_GetError()
        );
    }
    
    // Draw and fill the polygon on the off-screen buffer
    if (! ::SDL_SetRenderTarget(renderer, offscreenSurface)) {
        log_and_throw_SDL_failure<std::runtime_error>(
            "draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_SetRenderTarget failed: " + SDL_GetError()
        );
    }
    
    try { draw_outline(arg_outline_color); }
    catch (const std::exception& e) { 
        log_and_throw_SDL_failure<Logger::SeeAbove>(
            "draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            e.what()
        );
    }
    
    try { SdlUtils::flood_fill(renderer, centre_pos, arg_fill_color); }
    catch (const std::exception& e) {
        log_and_throw_SDL_failure<Logger::SeeAbove>(
            "draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            e.what()
        );
    }
    
    ::SDL_Texture* offscreenTexture = ::SDL_CreateTextureFromSurface(renderer, offscreenSurface);
    if (!offscreenTexture) {
        log_and_throw_SDL_failure<std::runtime_error>(
            "draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_CreateTextureFromSurface failed: " + SDL_GetError()
        );
    }
    
    // Copy the filled polygon from the off-screen buffer to the window renderer
    if (! ::SDL_SetRenderTarget(renderer, nullptr)) { // reset to default target
        log_and_throw_SDL_failure<std::runtime_error>(
            "draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_SetRenderTarget failed: " + SDL_GetError()
        );
    }
    if (! ::SDL_RenderTexture(renderer, offscreenTexture, NULL, NULL)) {
        log_and_throw_SDL_failure<std::runtime_error>(
            "draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_RenderTexture failed: " + SDL_GetError()
        );
    }
    
    // doNOT do ::SDL_RenderPresent(renderer);
    // Handle events and cleanup
    if (! ::SDL_DestroyTexture(offscreenTexture)) {
        log_and_throw_SDL_failure<std::runtime_error>(
            "draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_DestroyTexture failed: " + SDL_GetError()
        );
    }
}

// Implement member functions for the RegularPolygon class

template <size_t NumOfSides>
inline void RegularPolygon<NumOfSides>::calculate_vertices() {
    double angle_increment = 2 * M_PI / NumOfSides;
    for (size_t i = 0; i < NumOfSides; ++i) {
        double angle = i * angle_increment;
        Polygon<NumOfSides>::vertices[i] = Vector2D(static_cast<int>(radius * cos(angle)), static_cast<int>(radius * sin(angle)));
    }
}
template <size_t NumOfSides>
inline void RegularPolygon<NumOfSides>::calculate_pos_of_vertices() {
    for (size_t i = 0; i < NumOfSides; ++i) {
        pos_of_vertices[i] = {vertices[i].x + centre_pos.x, vertices[i].y + centre_pos.y};
    }
}

template <size_t NumOfSides>
inline void RegularPolygon<NumOfSides>::set_radius(int new_radius) {
    if (new_radius <= 0) {
        log_and_throw<std::invalid_argument>("set_radius(int new_radius)", "Radius must be positive");
    }
    radius = new_radius;
    calculate_pos_of_vertices();
}

} // namespace snake

#endif // POLYGON_INL

#endif // first_if