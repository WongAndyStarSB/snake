#include "ScreenObject.hpp"

using namespace snake;


void ScreenObject::setCentrePos(const SDL_FPoint& new_centre_pos) {
    helperThrowIfNonChangeable("setCentrePos");
    polygon->setCentrePos(new_centre_pos);
}
void ScreenObject::setPolygonNumOfSides(const size_t &new_num_of_sides) {
    helperThrowIfNonChangeable("setPolygonNumOfSides");
    polygon->setNumOfSides(new_num_of_sides);
}
void ScreenObject::setPolygonVertices(const std::vector<Vector2d> &vertices) {
    helperThrowIfNonChangeable("setPolygonVertices");
    polygon->setVertices(vertices);
}
void ScreenObject::setOutlineColor(const SDL_Color& new_outline_color) { 
    helperThrowIfNonChangeable("setOutlineColor");
    polygon->outline_color = new_outline_color;
}



// helpers

void ScreenObject::helperCheckWindowAndRenderer() const {
    if (!window) {
        logAndThrow<std::logic_error>(
            "ScreenObject constructor", 
            "Window is null"
        );
    }
    if (!renderer) {
        logAndThrow<std::logic_error>(
            "ScreenObject constructor", 
            "Renderer is null"
        );
    }
}
void ScreenObject::helperCheckIsPolygonNotNull() const {
    if (!polygon) {
        logAndThrow<std::logic_error>(
            "ScreenObject constructor", 
            "Polygon is null"
        );
    }
}
void ScreenObject::helperCheckPolygonWindowAndRenderer() const {
    if (window != polygon->getWindow()) {
        logAndThrow<std::logic_error>(
            "ScreenObject constructor", 
            "Polygon's window does not match ScreenObject's window"
        );
    }
    if (renderer != polygon->getRenderer()) {
        logAndThrow<std::logic_error>(
            "ScreenObject constructor", 
            "Polygon's renderer does not match ScreenObject's renderer"
        );
    }
}

void ScreenObject::helperThrowIfNonChangeable(const std::string& func_name) const {
    if (!is_changeable) {
        logAndThrow<std::logic_error>(
            "func_name",
            "attempts to change this when is_changeable is false"
        );
    }
}