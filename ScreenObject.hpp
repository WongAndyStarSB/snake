#ifndef SCREEN_OBJECT_HPP
#define SCREEN_OBJECT_HPP


#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <memory>
#include <functional>
#include "Polygon.hpp"

namespace snake {



class ScreenObject {
    using drawFuncPtrT = std::function<void(SDL_Renderer*)>;
    typedef void (*eventHandlerFuncPtrT)();

    private:

        SDL_Window* window; // non-owning
        SDL_Renderer* renderer; // non-owning
        std::unique_ptr<Polygon> polygon;
        std::unique_ptr<Polygon> show_polygon = nullptr;
        bool is_visible;
        bool is_clickable;
        eventHandlerFuncPtrT event_handler_func_ptr = nullptr;
        drawFuncPtrT draw_func_ptr = nullptr; // pointer to the draw function
        
        mutable bool is_changeable = true;

        // throw std::logic_error if window or renderer is null
        void helperCheckWindowAndRenderer() const;
        // throw std::logic_error if polygon is null
        void helperCheckIsPolygonNotNull() const;
        // throw std::logic_error if polygon's window or renderer does not match ScreenObject's window or renderer
        void helperCheckPolygonWindowAndRenderer() const;

        // throw
        void helperThrowIfNonChangeable(const std::string& func_name) const;

        void draw() const {
            draw_func_ptr(renderer);
        }

        void log(const std::string& where, const std::string& message, Logger::LogLevel level = Logger::INFO) const {
            Logger::log("ScreenObject::" + where, message, level);
        }
        template <typename ExceptionType>
        [[noreturn]] inline void logAndThrow(const std::string& where, const std::string& message) const {
            Logger::logAndThrow<ExceptionType>(
                "ScreenObject::" + where, 
                message
            );
            // unreachable
        }
    public:
        // Constructor
        ScreenObject(
            SDL_Window* arg_window, 
            SDL_Renderer* arg_renderer
        ) : window(arg_window), renderer(arg_renderer) 
        {
            helperCheckWindowAndRenderer();
            polygon = {std::make_unique<Polygon>(arg_window, arg_renderer)};
            draw_func_ptr = [this](SDL_Renderer* r){ this->polygon->draw(r); }; 
        }


        void render() const {
            if (is_visible) {
                this->draw();
            }
        }
        // getters
        bool getVisibility() const { return is_visible; }
        bool getClickability() const { return is_clickable; }
        const SDL_Window* getWindow() const { return window; }
        const SDL_Renderer* getRenderer() const { return renderer; }
        const Polygon* getPolygon() const { return polygon.get(); }
        const SDL_FPoint& getCentrePos() const { return polygon->getCentrePos(); }
        const SDL_Color& getOutlineColor() const { return polygon->outline_color; }
        const SDL_Color& getFillColor() const { return polygon->fill_color; }

        bool getIsChangeable() const { return is_changeable; }

        // setters
        void setCentrePos(const SDL_FPoint& new_centre_pos);
        void setPolygonNumOfSides(const size_t &new_num_of_sides);
        void setPolygonVertices(const std::vector<Math::Vector2d>& new_vertices);
        void setPolygon(Polygon* new_polygon) {
            helperCheckIsPolygonNotNull();
            polygon.reset(new_polygon);
            helperCheckWindowAndRenderer();
            helperCheckPolygonWindowAndRenderer();
        }
    
        void setOutlineColor(const SDL_Color& new_outline_color) { polygon->outline_color = new_outline_color; }
        void setFillColor(const SDL_Color& new_fill_color) { polygon->fill_color = new_fill_color; }
        void setVisibility(bool visible) { is_visible = visible; }
        void setClickability(bool clickable) { is_clickable = clickable; }

        void setIsChangeable(bool new_is_changeable) { is_changeable = new_is_changeable; }
        
        bool isInside(int x, int y) const { return polygon->isInside(x, y); }
        bool isFocusedByMouse(const SDL_Event& event) const { // return true if the event MOUSE is focused on this object
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
        bool isClickedByMouse(const SDL_Event& event) const {
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

        
        

}; // class ScreenObject


} // namespace snake



#endif // SCREEN_OBJECT_HPP