#ifndef SDL_UTILS_HPP
#define SDL_UTILS_HPP


#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_events.h>



namespace SdlUtils {


void flood_fill(::SDL_Renderer* renderer, const ::SDL_Point& starting_pos, const ::SDL_Color& result_color);
void flood_fill(::SDL_Renderer* renderer, int x, int y, const uint32_t& result_color_ARGB);

void flood_fill_F(::SDL_Renderer* renderer, const ::SDL_FPoint& starting_pos, const ::SDL_Color& result_color);
void flood_fill_F(::SDL_Renderer* renderer, float x, float y, const uint32_t& result_color_ARGB);

void flood_fill_target_color(::SDL_Renderer* renderer, const ::SDL_Point& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color);
void flood_fill_target_color(::SDL_Renderer* renderer, int x, int y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB);

void flood_fill_target_color_F(::SDL_Renderer* renderer, const ::SDL_FPoint& starting_pos, const ::SDL_Color& target_color, const ::SDL_Color& result_color);
void flood_fill_target_color_F(::SDL_Renderer* renderer, float x, float y, const uint32_t& target_color_ARGB, const uint32_t& result_color_ARGB);


}
#endif // SDL_UTILS_HPP