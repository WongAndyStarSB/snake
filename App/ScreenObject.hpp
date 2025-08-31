#ifndef SCREEN_OBJECT_HPP
#define SCREEN_OBJECT_HPP


#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <memory>
#include <functional>
#include <optional>
#include "Polygon.hpp"

namespace Display {



class ScreenObject {
    using DrawFuncPtrT = std::function<void()>;
    typedef void (*EventHandlerFuncPtrT)();

    protected:

        SDL_Window* window; // non-owning
        SDL_Renderer* renderer; // non-owning
        std::unique_ptr<Polygon> polygon;
        std::unique_ptr<Polygon> show_polygon = nullptr;
        bool is_visible = true;
        bool is_clickable = false;
        EventHandlerFuncPtrT event_handler_func_ptr = nullptr;
        DrawFuncPtrT draw_func = nullptr; // the draw function
        
        mutable bool is_changeable = true;

        // throw std::logic_error if window or renderer is null
        void helperCheckWindowAndRenderer() const;
        // throw std::logic_error if polygon is null
        void helperCheckIsPolygonNotNull() const;
        // throw std::logic_error if polygon's window or renderer does not match ScreenObject's window or renderer
        void helperCheckPolygonWindowAndRenderer() const;

        // throw
        void helperThrowIfNonChangeable(const std::string& func_name) const;

        // private setters

        // void privateSetCentrePos(const SDL_FPoint& new_centre_pos);


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
            try {
                helperCheckWindowAndRenderer();
                polygon = {std::make_unique<Polygon>(arg_window, arg_renderer)};
                draw_func = [this](){ this->polygon->draw(); }; 
            } catch (std::exception& e) {
                logAndThrow<Logger::SeeAbove>(
                    "ScreenObject constructor", 
                    e.what()
                );
            }
        }

        void setBasics(
            const Polygon& p, 
            bool visibility, 
            bool clickability,
            bool changeability = false
        );
        void setAll(
            const Polygon& p, 
            bool visibility, 
            bool clickability, 
            const Polygon& show_p,
            const EventHandlerFuncPtrT& ehfp,
            const DrawFuncPtrT& df,
            bool changeability = false
        );

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
        void setPolygon(const Polygon& new_polygon);
        void setPolygon(Polygon* new_polygon);
    
        void setOutlineColor(const SDL_Color& new_outline_color);
        void setFillColor(const SDL_Color& new_fill_color);
        void setVisibility(bool visible);
        void setClickability(bool clickable);

        void setIsChangeable(bool new_is_changeable) { is_changeable = new_is_changeable; }
        
        bool isInside(int x, int y) const;
        bool isFocusedByMouse(const SDL_Event& event) const;
        bool isClickedByMouse(const SDL_Event& event) const;

        void draw() const {
            draw_func();
        }
        
        

}; // class ScreenObject


} // namespace



#endif // SCREEN_OBJECT_HPP