#include "ScreenObject.hpp"

namespace Display {

void ScreenObject::setBasics(
    const Polygon& p, 
    bool visibility, 
    bool clickability,
    bool changeability
) {
    helperThrowIfNonChangeable("setBasics");
    polygon = std::make_unique<Polygon>(p);
    is_visible = visibility;
    is_clickable = clickability;

    event_handler_func_ptr = nullptr;
    ScreenObject::DrawFuncPtrT tmp = [this]()->void { 
        this->polygon->draw();
    };

    is_changeable = changeability;
}
void ScreenObject::setAll(
    const Polygon& p, 
    bool visibility, 
    bool clickability, 
    const Polygon& show_p,
    const EventHandlerFuncPtrT& ehfp,
    const DrawFuncPtrT& df,
    bool changeability = false
) {
    helperThrowIfNonChangeable("setAll");
    polygon = std::make_unique<Polygon>(p);
    is_visible = visibility;
    is_clickable = clickability;
    is_changeable = changeability;
}

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
void ScreenObject::setPolygon(const Polygon& new_polygon) {
    helperThrowIfNonChangeable("setPolygon");
    polygon = std::make_unique<Polygon>(new_polygon);
    helperCheckIsPolygonNotNull();
    helperCheckPolygonWindowAndRenderer();
}
void ScreenObject::setPolygon(Polygon* new_polygon) {
    helperThrowIfNonChangeable("setPolygon");
    polygon.reset(new_polygon);
    helperCheckIsPolygonNotNull();
    helperCheckPolygonWindowAndRenderer();
}

void ScreenObject::setOutlineColor(const SDL_Color& new_outline_color) { 
    helperThrowIfNonChangeable("setOutlineColor");
    polygon->outline_color = new_outline_color;
}
void ScreenObject::setFillColor(const SDL_Color& new_fill_color) { 
    helperThrowIfNonChangeable("setOutlineColor");
    polygon->fill_color = new_fill_color; 
}
void ScreenObject::setVisibility(bool visible) { 
    helperThrowIfNonChangeable("setOutlineColor");
    is_visible = visible; 
}
void ScreenObject::setClickability(bool clickable) { 
    helperThrowIfNonChangeable("setOutlineColor");
    is_clickable = clickable; 
}

bool ScreenObject::isInside(int x, int y) const { 
    return polygon->isInside(x, y);
 }
bool ScreenObject::isFocusedByMouse(const SDL_Event& event) const { 
    // return true if the event MOUSE is focused on this object
    if (!is_visible) {
        return false;
    }
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        if (polygon->isInside(event.motion.x, event.motion.y)) {
            log("isFocusedByMouse", "focused by mouse at (" + std::to_string(event.motion.x) + ", " + std::to_string(event.motion.y) + ")");
            return true;
        }
    }
}
bool ScreenObject::isClickedByMouse(const SDL_Event& event) const {
    if (!is_clickable || !is_visible) {
        return false;
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int x = event.button.x;
        int y = event.button.y;
        if (polygon->isInside(x, y)) {
            log("isClickedByMouse", "clicked by mouse at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
            return true;
        }
    }
    return false;
}



// helpers

void ScreenObject::helperCheckWindowAndRenderer() const {
    if (!window) {
        logAndThrow<std::logic_error>(
            "helperCheckWindowAndRenderer", 
            "Window is null"
        );
    }
    if (!renderer) {
        logAndThrow<std::logic_error>(
            "helperCheckWindowAndRenderer", 
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

} // namespace screen