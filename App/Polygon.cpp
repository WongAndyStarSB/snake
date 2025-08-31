#include "Polygon.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include <vector>

#include "../Math/ComplexNumber.hpp"

namespace Display {

using Vector2d = Math::Vector2d;

// static
::SDL_Window* Polygon::default_window = nullptr;
::SDL_Renderer* Polygon::default_renderer = nullptr;
::SDL_Color Polygon::default_outline_color = {255, 255, 255, 255};
::SDL_Color Polygon::default_fill_color = {0, 0, 0, 100}; // default fill color is halfly-transparent
bool Polygon::default_fill = true;


void Polygon::setDefaultWindowAndRenderer(::SDL_Window* const &w, ::SDL_Renderer* const &r) {
    static uint16_t count = 0;
    if (w == default_window && r == default_renderer) {
        return;
    }
    if (count > 0) {
        static_log(
            "setDefaultWindowAndRenderer(::SDL_Window* w, ::SDL_Renderer* r)", 
            "default window and renderer have been resetted",
            Logger::WARNING_2
        );
    }
    default_window = w;
    default_renderer = r;
    count++;
}

void Polygon::setDefaultColors(const ::SDL_Color &outline_color_, const ::SDL_Color &fill_color_, bool fill_) {
    default_outline_color = outline_color_;
    default_fill_color = fill_color_;
    default_fill = fill_;
}


::SDL_Window* Polygon::getDefaultWindow() {
    return default_window;
}

::SDL_Renderer* Polygon::getDefaultRenderer() {
    return default_renderer;
}

// constructors

Polygon::Polygon() 
    : window(default_window), renderer(default_renderer), outline_color(default_outline_color), fill_color(default_fill_color), fill(default_fill)
    {
    helperThrowIfNullWindowOrRenderer("Polygon() /* default constructor */");
}

Polygon::Polygon(const ::SDL_FPoint& arg_centre_pos, const size_t& arg_num_of_sides
    ) : centre_pos(arg_centre_pos), num_of_sides(arg_num_of_sides), 
        window(default_window), renderer(default_renderer), outline_color(default_outline_color), fill_color(default_fill_color), fill(default_fill)
    {
    try {
        helperThrowIfNullWindowOrRenderer();
        helperInitVerticesFromNumOfSides();
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "Polygon(const ::SDL_FPoint& arg_centre_pos, const size_t& arg_num_of_sides)",
            e.what()
        );
    }
}

Polygon::Polygon(const ::SDL_FPoint& arg_centre_pos, const std::vector<Vector2d>& arg_vertices
    ) : centre_pos(arg_centre_pos), num_of_sides(arg_vertices.size()), vertices(arg_vertices),
        window(default_window), renderer(default_renderer), outline_color(default_outline_color), fill_color(default_fill_color), fill(default_fill) 
    {
    try {
        helperThrowIfNullWindowOrRenderer();
        helperUpdateVertexPositions();
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "Polygon(const ::SDL_FPoint& arg_centre_pos, const size_t& arg_num_of_sides)",
            e.what()
        );
    };
}

Polygon::Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer
    ) : window(arg_window), renderer(arg_renderer), 
        outline_color(default_outline_color), fill_color(default_fill_color), fill(default_fill)
    {
    helperThrowIfNullWindowOrRenderer("Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer)");
}
Polygon::Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer, const ::SDL_FPoint& arg_centre_pos, const size_t& arg_num_of_sides
    ) : window(arg_window), renderer(arg_renderer), centre_pos(arg_centre_pos), num_of_sides(arg_num_of_sides), 
        outline_color(default_outline_color), fill_color(default_fill_color), fill(default_fill)
    {
    try {
        helperThrowIfNullWindowOrRenderer();
        helperInitVerticesFromNumOfSides(false, Vector2d());
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer, const ::SDL_FPoint& arg_centre_pos, const size_t& arg_num_of_sides)",
            e.what()
        );
    }
}
Polygon::Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer, const ::SDL_FPoint& arg_centre_pos, const std::vector<Vector2d>& arg_vertices)
    : window(arg_window), renderer(arg_renderer), centre_pos(arg_centre_pos), num_of_sides(arg_vertices.size()), vertices(arg_vertices)
    {
    try {
    helperThrowIfNullWindowOrRenderer();
    helperUpdateVertexPositions();
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "Polygon(::SDL_Window* arg_window, ::SDL_Renderer* arg_renderer, const ::SDL_FPoint& arg_centre_pos, const std::vector<Vector2d>& arg_vertices)",
            e.what()
        );
    }
}



// getters


// setters

void Polygon::setCentrePos(const SDL_FPoint& new_centre_pos) {
    centre_pos = new_centre_pos;
    // Update the positions of the vertices based on the new centre position
    for (size_t i = 0; i < num_of_sides; ++i) {
        pos_of_vertices[i] = SDL_FPoint {
            static_cast<float>(vertices[i].x()) + centre_pos.x, 
            static_cast<float>(vertices[i].y()) + centre_pos.y
        };
    }
}
void Polygon::setVertices(const std::vector<Vector2d>& new_vertices) {
    if (new_vertices.size() < 3) {
        logAndThrow<std::logic_error>("setVertices", "number of vertices must be at least 3");
    }
    vertices = new_vertices;
    num_of_sides = vertices.size();
    pos_of_vertices.resize(num_of_sides);
    helperUpdateVertexPositions();
}
void Polygon::setVerticesData(const std::vector<Vector2d>& new_vertices) {
    if (new_vertices.size() != num_of_sides) {
        logAndThrow<std::logic_error>("setVerticesData", "Number of vertices must be the same as num_of_sides");
    }
    vertices = new_vertices;
    helperUpdateVertexPositions();
}
void Polygon::setNumOfSides(const size_t &new_num_of_sides, bool clear_data, const Vector2d &default_val_for_vertices) {
    if (new_num_of_sides < 3) {
        logAndThrow<std::logic_error>("setNumOfSides", "number of sides must be at least 3");
    }
    num_of_sides = new_num_of_sides;
    helperInitVerticesFromNumOfSides(clear_data, default_val_for_vertices);
}
void Polygon::helperUpdateVertexPositions() {
    for (size_t i = 0; i < num_of_sides; ++i) {
        pos_of_vertices[i] = SDL_FPoint {
            static_cast<float>(vertices[i].x()) + centre_pos.x, 
            static_cast<float>(vertices[i].y()) + centre_pos.y
        };
    }
}

// public edit functions

void Polygon::enlarge(Math::Fraction ratio) {
    for (Vector2d &vertex : vertices) {
        vertex *= ratio.get_in_double();
    }
    helperUpdateVertexPositions();
}
void Polygon::rotate(const Math::Angle& radian) {
    this->angle += radian;
    // Rotate each vertex using complex number multiplication (Euler's formula)
    Math::ComplexNumber rotator = Math::ComplexNumber::createFromPolar(1.0, radian);
    for (Vector2d &vertex : vertices) {
        vertex = (Math::ComplexNumber(vertex) * rotator).toVector2d();
    }
    helperUpdateVertexPositions();
}
void Polygon::rotate(const Math::ComplexNumber& complex) {
    this->angle += complex.getAngle();
    for (Vector2d &vertex : vertices) {
        vertex = (Math::ComplexNumber(vertex) * complex).toVector2d();
    }
    helperUpdateVertexPositions();
}
void Polygon::rotate(const Math::RotationMatrix<2>& rotate_mat) {
    this->angle += rotate_mat.getAngleConstReference();
    for (Vector2d &vertex : vertices) {
        vertex = rotate_mat * vertex.getBaseConstReference();
    }
    helperUpdateVertexPositions();
}

// public draw functions

void Polygon::draw(SDL_Window* arg_window, SDL_Renderer* arg_renderer) {
    if (arg_renderer != nullptr && arg_window == nullptr && fill) {
        logAndThrow<std::logic_error>("draw", "drawWithFill needs window");
    }
    arg_renderer = arg_renderer == nullptr ? renderer : arg_renderer;
    arg_window = arg_window == nullptr ? window : arg_window;
    
    try {
        if (fill) {
            helperDrawWithFill(arg_window, arg_renderer, outline_color, fill_color);
        } else {
            helperDrawOutline(arg_renderer, outline_color);
        }
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "draw(SDL_Window* arg_window, SDL_Renderer* arg_renderer)",
            e.what()
        );
    }
}

void Polygon::draw() const {
    try {
        if (fill) {
            helperDrawWithFill(window, renderer, outline_color, fill_color);
        } else {
            helperDrawOutline(renderer, outline_color);
        }
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "draw() const",
            e.what()
        );
    }
}
void Polygon::draw(bool arg_fill) const {
    try {
        if (arg_fill) {
            helperDrawWithFill(window, renderer, outline_color, fill_color);
        } else {
            helperDrawOutline(renderer, outline_color);
        }
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "draw(bool arg_fill) const",
            e.what()
        );
    }
}

void Polygon::drawWithoutFill() const {
    try {
        helperDrawOutline(renderer, outline_color);
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "drawWithoutFill() const",
            e.what()
        );
    }
}
void Polygon::drawWithoutFill(const ::SDL_Color& arg_outline_color) const {
    try {
        helperDrawOutline(renderer, arg_outline_color);
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "drawWithoutFill(const ::SDL_Color& arg_outline_color) const",
            e.what()
        );
    }
}

void Polygon::drawWithFill() const {
    try {
        helperDrawWithFill(window, renderer, outline_color, fill_color);
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "drawWithFill() const",
            e.what()
        );
    }
}
void Polygon::drawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const {
    try {
        helperDrawWithFill(window, renderer, arg_outline_color, arg_fill_color);
    } catch (const std::exception& e) {
        logAndThrow<Logger::SeeAbove>(
            "drawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const",
            e.what()
        );
    }
}

bool Polygon::isInside(const int& pos_x, const int& pos_y) const noexcept {
    // Check if the point is inside the polygon using the ray-casting algorithm
    int count = 0;
    for (size_t i = 0; i < num_of_sides; ++i) {
        const SDL_FPoint& v1 = pos_of_vertices[i];
        const SDL_FPoint& v2 = pos_of_vertices[(i + 1) % num_of_sides];
        // judging that does the horizontal ray formed by casting the pos to right intersects with the line_segment of v1 and v2
        if (((v1.y > pos_y) != (v2.y > pos_y)) && // height of pos is in between v1 and v2
            (pos_x < ((v2.x - v1.x) * (pos_y - v1.y) / (v2.y - v1.y) + v1.x)) // pos is at the left of the line_segment v2-v1
        ) {
            count++;
        }
    }
    return count % 2 == 1;
}

void Polygon::helperConstructDos() {
    // Checks
    helperThrowIfNullWindowOrRenderer("helperConstructDos");
    // Initialize vertices and positions of vertices
    vertices.resize(num_of_sides);
    pos_of_vertices.resize(num_of_sides);
}
void Polygon::helperThrowIfNullWindowOrRenderer(const std::string& func_name) const {
    if (!window) {
        logAndThrow<std::logic_error>(func_name, "Window is null");
    }
    if (!renderer) {
        logAndThrow<std::logic_error>(func_name, "Renderer is null");
    }
}
void Polygon::helperThrowIfNumOfSidesIsNotValid(const std::string& func_name) const {
    log("helper_throw_if_num_of_sides_is_not_valid", "this should be a no use function", Logger::WARNING_3);
    if (num_of_sides < 3) {
        logAndThrow<std::logic_error>(func_name, "Number of sides must be at least 3");
    }
}

void Polygon::helperInitVerticesFromNumOfSides(bool clear_data, const Vector2d& default_val_for_vertices) {
    if (clear_data) {
        vertices.assign(num_of_sides, default_val_for_vertices);
    } else {
        vertices.resize(num_of_sides, default_val_for_vertices);
    }
    pos_of_vertices.resize(num_of_sides);
    helperUpdateVertexPositions();
}

void Polygon::helperDrawOutline(SDL_Renderer* arg_renderer, const ::SDL_Color& arg_color) const {
    if (! ::SDL_SetRenderDrawColor(arg_renderer, arg_color.r, arg_color.g, arg_color.b, arg_color.a)) {
        logAndThrow_SDL_failure<std::runtime_error>(
            "helperDrawOutline(const ::SDL_Color& arg_color) const", 
            "SDL_SetRenderDrawColor"
        );
    }
    
    if (! ::SDL_RenderLines(arg_renderer, pos_of_vertices.data(), pos_of_vertices.size())) {
        logAndThrow_SDL_failure<std::runtime_error>(
            "draw_outline(const ::SDL_Color& arg_color) const", 
            "SDL_RenderLines"
        );
    }
}

void Polygon::helperDrawWithFill(SDL_Window* arg_window, SDL_Renderer* arg_renderer, const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color) const {
    if (! ::SDL_SetRenderDrawColor(arg_renderer, arg_fill_color.r, arg_fill_color.g, arg_fill_color.b, arg_fill_color.a)) {
        logAndThrow_SDL_failure<std::runtime_error>(
            "fill(const ::SDL_Color& arg_fill_color)", 
            "SDL_SetRenderDrawColor"
        );
    }
    // Create an off-screen buffer to draw the polygon
    int windowWidth, windowHeight;
    if (! ::SDL_GetWindowSize(arg_window, &windowWidth, &windowHeight)) {
        logAndThrow_SDL_failure<std::runtime_error>(
            "helperDrawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_GetWindowSize"
        );
    }
    
    ::SDL_Texture* offscreenTexture = ::SDL_CreateTexture(arg_renderer, ::SDL_PIXELFORMAT_RGBA32, ::SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
    if (!offscreenTexture) {
        logAndThrow_SDL_failure<std::runtime_error>(
            "helperDrawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_CreateTexture"
        );
    }
    
    // Draw and fill the polygon on the off-screen buffer
    if (! ::SDL_SetRenderTarget(arg_renderer, offscreenTexture)) {
        logAndThrow_SDL_failure<std::runtime_error>(
            "helperDrawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_SetRenderTarget"
        );
    }
    
    try { helperDrawOutline(renderer, arg_outline_color); }
    catch (const std::exception& e) { 
        logAndThrow_SDL_failure<Logger::SeeAbove>(
            "helperDrawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            e.what()
        );
    }
    
    try { SdlUtils::flood_fill_F(arg_renderer, centre_pos, arg_fill_color); }
    catch (const std::exception& e) {
        logAndThrow_SDL_failure<Logger::SeeAbove>(
            "helperDrawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            e.what()
        );
    }
    
    
    
    
    // Copy the filled polygon from the off-screen buffer to the window renderer
    if (! ::SDL_SetRenderTarget(renderer, nullptr)) { // reset to default target
        logAndThrow_SDL_failure<std::runtime_error>(
            "helperDrawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_SetRenderTarget"
        );
    }
    if (! ::SDL_RenderTexture(renderer, offscreenTexture, NULL, NULL)) {
        logAndThrow_SDL_failure<std::runtime_error>(
            "helperDrawWithFill(const ::SDL_Color& arg_outline_color, const ::SDL_Color& arg_fill_color)", 
            "SDL_RenderTexture"
        );
    }
    
    // doNOT do ::SDL_RenderPresent(renderer);
    // Handle events and cleanup
    ::SDL_DestroyTexture(offscreenTexture);
}



}