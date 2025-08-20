#ifndef POLYGON_HPP
#define POLYGON_HPP


#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

#include <string>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <numbers>

#include "Logger.hpp"
#include "Math/Math.hpp"
#include "SdlUtils.hpp"

namespace snake {


class Polygon { // positioned polygon with NumOfSides sides
private:
    ::SDL_Window* window; // non-owning // don't delete
    ::SDL_Renderer* renderer; // non-owning // don't delete
    ::SDL_FPoint centre_pos = {0, 0}; // default centre position is (0, 0)
    Math::Angle angle { 0 };
    size_t num_of_sides = 0; // number of sides of the polygon
    std::vector<Vector2d> vertices; // vertices of the polygon
    std::vector<SDL_FPoint> pos_of_vertices; // positions of vertices of the polygon
    static ::SDL_Window* default_window;
    static ::SDL_Renderer* default_renderer;
public:
    static ::SDL_Color default_outline_color;
    static ::SDL_Color default_fill_color;
    static bool default_fill;
    mutable ::SDL_Color outline_color = default_outline_color;
    mutable ::SDL_Color fill_color = default_fill_color;
    mutable bool fill = true; // whether the polygon is filled or not

    // # public static methods

    static void setDefaultWindowAndRenderer(::SDL_Window* const &w, ::SDL_Renderer* const &r);
    static void setDefaultColors(const ::SDL_Color& outline_color_, const ::SDL_Color& fill_color_, bool fill);
    
    static ::SDL_Window* getDefaultWindow();
    static ::SDL_Renderer* getDefaultRenderer();

    // # constructors

    explicit Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer);
    explicit Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer, const ::SDL_FPoint& arg_centre_pos, const size_t& arg_num_of_sides);
    explicit Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer, const ::SDL_FPoint& arg_centre_pos, const std::vector<Vector2d>& arg_vertices);

    /* 
    constructor
    note: this constructor uses static defaults
    */
    explicit Polygon(const ::SDL_FPoint& arg_centre_pos, const size_t& arg_num_of_sides);
    // uses static default variables
    explicit Polygon(const ::SDL_FPoint& arg_centre_pos, const std::vector<Vector2d>& arg_vertices);

    // default constructor
    explicit Polygon();

    // desturctor
    virtual ~Polygon() = default;

    const ::SDL_Window *getWindow() const noexcept { return window; }
    const ::SDL_Renderer *getRenderer() const noexcept { return renderer; }
    const ::SDL_FPoint &getCentrePos() const noexcept { return centre_pos; }
    const std::vector<Vector2d> &getVertices() const noexcept { return vertices; }
    const std::vector<::SDL_FPoint> &getPosOfVertices() const noexcept { return pos_of_vertices; }

    void setCentrePos(const SDL_FPoint& new_centre_pos);
    void setVertices(const std::vector<Vector2d>& new_vertices);
    void setVerticesData(const std::vector<Vector2d>& new_vertices);
    void setNumOfSides(const size_t &new_num_of_sides, bool clear_data = false, const Vector2d &default_val_for_vertices = Vector2d());
    
    void enlarge(Math::Fraction ratio);
    void rotate(const Math::Angle& radian);
    void rotate(const Math::ComplexNumber& complex);
    void rotate(const Math::RotationMatrix<2>& rotate_mat);

    void draw(SDL_Window* arg_window, SDL_Renderer* arg_renderer);
    void draw() const { draw(fill); }
    void draw(bool arg_fill) const;
    void drawWithoutFill() const;
    void drawWithoutFill(const ::SDL_Color& arg_outline_color) const;
    void drawWithFill() const;
    void drawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const;
    
    // Check if the point is inside the polygon
    bool isInside(const int& pos_x, const int& pos_y) const noexcept;

private:
    
    void log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) const {
        Logger::log("Polygon::" + where, message, level);
    }
    static void static_log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) {
        Logger::log("Polygon::" + where, message, level);
    }
    
    template <typename ExceptionType> \
    [[noreturn]] inline void logAndThrow(const std::string& where, const std::string& message) const {
        Logger::logAndThrow<ExceptionType>(
            "Polygon::" + where
                + "\nmember status {"
                  "\n  renderer: " + std::to_string(renderer)
                + "\n  vertices:" + Vector2d::vector_to_string(vertices, false)
                + "\n  centre_pos(" + centre_pos.to_string()
                + ")\n  mutable outline_color(" + outline_color.to_string()
                + ")\n  mutable fill_color(" + fill_color.to_string() 
                + ")\n}", 
            message
        );
    }
    
    template <typename ExceptionType = std::runtime_error> \
    [[noreturn]] inline void logAndThrow_SDL_failure(const std::string& where, const std::string& sdl_func_name) const {
        this->logAndThrow<ExceptionType>(
            where, 
            sdl_func_name + " failed: " + SDL_GetError()
        );
    }

    void helperConstructDos();
    void helperThrowIfNullWindowOrRenderer(const std::string& func_name = "helperThrowIfNullWindowOrRenderer") const;
    void helperThrowIfNumOfSidesIsNotValid(const std::string& func_name) const;

    void helperInitVerticesFromNumOfSides(bool clear_data = false, const Vector2d& default_val_for_vertices = Vector2d());
    // Update the positions of the vertices based on the current centre position
    // It should only be used after pos_of_vertices has the correct size
    void helperUpdateVertexPositions();

    // this function is used to draw the outline of the shape
    // caution: it does not check the renderer
    void helperDrawOutline(SDL_Renderer* arg_renderer, const ::SDL_Color& arg_color) const;
    // this function is used to draw the polygon (including outline and fill)
    // caution: it does not check the renderer
    void helperDrawWithFill(SDL_Window* arg_window, SDL_Renderer* renderer, const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const;
};

template <size_t NumOfSides>
class RegularPolygon : public Polygon<NumOfSides> {
private:
    int radius;
    void calculateVertices();
    
    // Update the positions of the vertices based on the current centre position
    void calculatePosOfVertices();

    void log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) const {
        Logger::log("RegularPolygon<" + std::to_string(NumOfSides) + ">::" + where, message, level);
    }
    template <typename ExceptionType>
    [[noreturn]] inline void logAndThrow(const std::string& where, const std::string& message) {
        Logger::logAndThrow<ExceptionType>("RegularPolygon<" + std::to_string(NumOfSides) + ">::" + where, message);
    }

public:
    inline RegularPolygon(SDL_Window* arg_window, SDL_Renderer* arg_renderer, const Pos2D& arg_centre_pos, int arg_radius)
        : Polygon<NumOfSides>(arg_window, arg_renderer, arg_centre_pos), radius(arg_radius) {
        if (radius <= 0) {
            logAndThrow<std::invalid_argument>(
                "RegularPolygon(SDL_Renderer* arg_renderer, int arg_radius) /* constructor */", 
                "Radius must be positive"
            );
        }
        calculateVertices();
        calculatePosOfVertices();
    }
    inline RegularPolygon(SDL_Window* arg_window, SDL_Renderer* arg_renderer, const Pos2D& arg_centre_pos, int arg_radius, const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color = Polygon<NumOfSides>::default_fill_color)
        : Polygon<NumOfSides>(arg_window, arg_renderer, arg_centre_pos, arg_outline_color, arg_fill_color), radius(arg_radius) {
        if (radius <= 0) {
            logAndThrow<std::invalid_argument>(
                "RegularPolygon(SDL_Renderer* arg_renderer, int arg_radius, const Pos2D& arg_centre_pos, const ::SDL_Color& arg_color) /* constructor */", 
                "Radius must be positive"
            );
        }
        calculateVertices();
        calculatePosOfVertices();
    }
    
    void setRadius(int new_radius);

    void draw(bool fill = true) const {
        Polygon<NumOfSides>::draw(fill);
    }
    void draw(bool fill, const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const {
        Polygon<NumOfSides>::draw(fill, arg_outline_color, arg_fill_color);
    }


};

}


#endif // POLYGON_HPP