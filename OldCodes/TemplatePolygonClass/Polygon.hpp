#if 0
#ifndef POLYGON_HPP
#define POLYGON_HPP


#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include <string>
#include <stdexcept>
#include <cmath>
#include <array>
#include <numbers>

#include "Logger.hpp"
#include "Vector2D.hpp"
#include "Pos2D.hpp"
#include "SdlUtils.hpp"

namespace snake {

template <size_t NumOfSides>
class Polygon { // positioned polygon with NumOfSides sides
private:
    ::SDL_Window* window; // non-owning // don't delete
    ::SDL_Renderer* renderer; // non-owning // don't delete
    ::SDL_Point centre_pos = {0, 0}; // default centre position is (0, 0)
    std::array<Vector2D, NumOfSides> vertices; // vertices of the polygon
    std::array<SDL_Point, NumOfSides> pos_of_vertices; // positions of vertices of the polygon
    
    void log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) const {
        Logger::log("Polygon<" + std::to_string(NumOfSides) + ">::" + where, message, level);
    }
    template <typename ExceptionType>
    [[noreturn]] inline void log_and_throw(const std::string& where, const std::string& message) {
        Logger::log_and_throw<ExceptionType>(
            "Polygon<" + std::to_string(NumOfSides) + ">::" + where
                + "\nmember status {"
                  "\n  renderer: " + std::to_string(renderer)
                + "\n  vertices:" + Vector2D::vector_to_string(vertices, false)
                + "\n  centre_pos(" + centre_pos.to_string()
                + ")\n  mutable outline_color(" + outline_color.to_string()
                + ")\n  mutable fill_color(" + fill_color.to_string() 
                + ")\n}", 
            message
        );
    }
    template <typename ExceptionType = std::runtime_error>
    [[noreturn]] inline void log_and_throw_SDL_failure(const std::string& where, const std::string& sdl_func_name) {
        Logger::log_and_throw<ExceptionType>(
            "SdlUtils::" + where, 
            sdl_func_name + " failed: " + SDL_GetError()
        );
    }

    // this function is used to draw the outline of the shape
    // caution: it does not check the renderer
    inline void draw_outline(const ::SDL_Color& arg_color) const;
    // this function is used to draw the polygon (including outline and fill)
    // caution: it does not check the renderer
    void draw_with_fill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const;

public:
    static ::SDL_Color default_fill_color = {0, 0, 0, 100}; // default fill color is halfly-transparent
    mutable ::SDL_Color outline_color = {255, 255, 255, 255}; // default color is white
    mutable ::SDL_Color fill_color = default_fill_color;
    mutable bool fill = true; // whether the polygon is filled or not

    explicit Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer) 
        : window(arg_window), renderer(arg_renderer) {
        if (!arg_window) {
            log_and_throw<std::logic_error>("Polygon constructor", "Window is null");
        }
        if (!arg_renderer) {
            log_and_throw<std::logic_error>("Polygon constructor", "Renderer is null");
        }
    }
    explicit Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer, const ::SDL_Point& arg_centre_pos)
        : window(arg_window), renderer(arg_renderer), centre_pos(arg_centre_pos)
        {
        if (!window) {
            log_and_throw<std::logic_error>("Polygon constructor", "Window is null");
        }
        if (!renderer) {
            log_and_throw<std::logic_error>("Polygon constructor", "Renderer is null");
        }
    }
    explicit Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer, bool arg_fill, const ::SDL_Point& arg_centre_pos, const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color = default_fill_color)
        : renderer(arg_renderer), centre_pos(arg_centre_pos), is_filled(arg_fill), outline_color(arg_outline_color), fill_color(arg_fill_color) 
        {
        if (!window) {
            log_and_throw<std::logic_error>("Polygon constructor", "Window is null");
        }
        if (!renderer) {
            log_and_throw<std::logic_error>("Polygon constructor", "Renderer is null");
        }
    }
    virtual ~Polygon() = default;

    void set_centre_pos(const Pos2D& new_centre_pos);

    void draw(bool arg_fill = fill) const;
    void draw(bool arg_fill = fill, const ::SDL_Color& arg_outline_color = outline_color, const ::SDL_Color& arg_fill_color = fill_color) const;

    bool is_inside(const int& pos_x, const int& pos_y) const noexcept {
        // Check if the point is inside the polygon using the ray-casting algorithm
        int count = 0;
        for (size_t i = 0; i < NumOfSides; ++i) {
            const SDL_Point& v1 = pos_of_vertices[i];
            const SDL_Point& v2 = pos_of_vertices[(i + 1) % NumOfSides];
            // judging that does the horizontal ray formed by casting the pos to right intersects with the line_segment of v1 and v2
            if (((v1.y > pos_y) != (v2.y > pos_y)) && // height of pos is in between v1 and v2
                (pos_x < ((v2.x - v1.x) * (pos_y - v1.y) / (v2.y - v1.y) + v1.x)) // pos is at the left of the line_segment v2-v1
            ) {
                count++;
            }
        }
        return count % 2 == 1;
    }
};

template <size_t NumOfSides>
class RegularPolygon : public Polygon<NumOfSides> {
private:
    int radius;
    void calculate_vertices();
    void calculate_pos_of_vertices();

    void log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) const {
        Logger::log("RegularPolygon<" + std::to_string(NumOfSides) + ">::" + where, message, level);
    }
    template <typename ExceptionType>
    [[noreturn]] inline void log_and_throw(const std::string& where, const std::string& message) {
        Logger::log_and_throw<ExceptionType>("RegularPolygon<" + std::to_string(NumOfSides) + ">::" + where, message);
    }

public:
    inline RegularPolygon(SDL_Window* arg_window, SDL_Renderer* arg_renderer, const Pos2D& arg_centre_pos, int arg_radius)
        : Polygon<NumOfSides>(arg_window, arg_renderer, arg_centre_pos), radius(arg_radius) {
        if (radius <= 0) {
            log_and_throw<std::invalid_argument>(
                "RegularPolygon(SDL_Renderer* arg_renderer, int arg_radius) /* constructor */", 
                "Radius must be positive"
            );
        }
        calculate_vertices();
        calculate_pos_of_vertices();
    }
    inline RegularPolygon(SDL_Window* arg_window, SDL_Renderer* arg_renderer, const Pos2D& arg_centre_pos, int arg_radius, const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color = Polygon<NumOfSides>::default_fill_color)
        : Polygon<NumOfSides>(arg_window, arg_renderer, arg_centre_pos, arg_outline_color, arg_fill_color), radius(arg_radius) {
        if (radius <= 0) {
            log_and_throw<std::invalid_argument>(
                "RegularPolygon(SDL_Renderer* arg_renderer, int arg_radius, const Pos2D& arg_centre_pos, const ::SDL_Color& arg_color) /* constructor */", 
                "Radius must be positive"
            );
        }
        calculate_vertices();
        calculate_pos_of_vertices();
    }
    
    void set_radius(int new_radius);

    void draw(bool fill = true) const {
        Polygon<NumOfSides>::draw(fill);
    }
    void draw(bool fill, const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const {
        Polygon<NumOfSides>::draw(fill, arg_outline_color, arg_fill_color);
    }


};

}

#include "Polygon.inl" // include the inline implementation

#endif // POLYGON_HPP

#endif