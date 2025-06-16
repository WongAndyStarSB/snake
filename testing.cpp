#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
//#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_timer.h>

int i;


bool running = true;

int main(int, char**) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	};

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Color background_color = { 0, 150, 0, 255 };
	SDL_Color rect_color = { 255, 0, 0, 255 };
	SDL_Color txt_color = { 0, 100, 200, 255 };

	if (!SDL_CreateWindowAndRenderer("snake", 640, 480, 0, &window, &renderer)) {
		SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	
	// TTF_Font* txt_font = TTF_OpenFont("arial.ttf", 24);
	// SDL_Surface* txt_surface = TTF_RenderText_Solid(txt_font, "Hello, SDL!", 0,  txt_color);
	// SDL_Texture* txt_texture = SDL_CreateTextureFromSurface(renderer, txt_surface);
	SDL_FRect rect = { 100, 100, 50, 70 };
	while (running) {
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
				break;
			} else if (event.type == SDL_EVENT_KEY_DOWN) {
				if (event.key.scancode == SDL_SCANCODE_W) {
					rect.y -= 5;
				} else if (event.key.scancode == SDL_SCANCODE_S) {
					rect.y += 5;
				} else if (event.key.scancode == SDL_SCANCODE_A) {
					rect.x -= 5;
				} else if (event.key.scancode == SDL_SCANCODE_D) {
					rect.x += 5;
				} else if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
					running = false;
					break;
				} else if (event.key.scancode == SDL_SCANCODE_SPACE) {
					SDL_Log("space pressed");
				} else {
					SDL_Log("other key pressed");
				}
			}
		}
		SDL_RenderClear(renderer);

		if (!SDL_SetRenderDrawColor(renderer, rect_color.r, rect_color.g, rect_color.b, rect_color.a)) {
			SDL_Log("Failed to set render draw color: %s", SDL_GetError());
		}
		if (!SDL_RenderFillRect(renderer, &rect)) {
			SDL_Log("Failed to fill rect: %s", SDL_GetError());
		}

		
		// SDL_FRect txt_rect = { 200, 200, static_cast<float>(txt_surface->w), static_cast<float>(txt_surface->h) };
		// SDL_RenderTexture(renderer, txt_texture, &txt_rect, NULL);


		
		if (!SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a)) {
			SDL_Log("Failed to set render draw color: %s", SDL_GetError());
		}
		SDL_RenderPresent(renderer);
		

	}

	//clear font resources
	
	// SDL_DestroySurface(txt_surface);
	// SDL_DestroyTexture(txt_texture);
	// TTF_CloseFont(txt_font);
	// TTF_Quit();
	

	//clear and quit
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	std::cout << "program finished" << std::endl;
	return 0;
}