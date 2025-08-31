#ifndef COMPOSITE_SCREEN_OBJECT_HPP
#define COMPOSITE_SCREEN_OBJECT_HPP


#include <vector>
#include <memory>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include "ScreenObject.hpp"
#include "Logger.hpp"

namespace Display {

class CompositeScreenObject : protected ScreenObject {

protected:
    std::vector<std::unique_ptr<ScreenObject>> objs;
public:
    CompositeScreenObject(SDL_Window* w, SDL_Renderer* r)
        try: ScreenObject(w, r) {
            
        } catch (std::exception& e) {
            logAndThrow<Logger::SeeAbove>(
                "CompositeScreenObject /* constructor */", e.what()
            );
    }
    


private:
    template <typename ExceptionType>
    [[noreturn]] void logAndThrow(
        const std::string& where, const std::string& what
    ) const {
        Logger::logAndThrow<ExceptionType>(
            "CompositeScreenObject::" + where, what
        );
    }

}; // class CompositeScreenObject

} // namespace screen

#endif // COMPOSITE_SCREEN_OBJECT_HPP