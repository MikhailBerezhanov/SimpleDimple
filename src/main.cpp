// mingw-specific
#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif
// for initializing and shutdown functions
#include <SDL2/SDL.h>

// for rendering images and graphics on screen
// #include <SDL2/SDL_image.h>

// for using SDL_Delay() functions
#include <SDL2/SDL_timer.h>

#include <iostream>
#include <stdexcept>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[])
{

	try
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
		}
		auto window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          0);
		if(!window){
			throw std::runtime_error("SDL_CreateWindow error! SDL_Error: " + std::string(SDL_GetError()));
		}

		auto screenSurface = SDL_GetWindowSurface(window);
		if (!screenSurface){
			throw std::runtime_error("Init surface error! SDL_Error: " + std::string(SDL_GetError()));
		}
		// Fill the surface white
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

		// Update the surface
		SDL_UpdateWindowSurface(window);

		// Hack to get window to stay up
		SDL_Event e;
		bool quit = false;
		while (!quit)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				if (e.type == SDL_KEYDOWN)
				{
					quit = true;
				}
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					quit = true;
				}
			}
		}

		std::cout << "End of loop!" << std::endl;

		// Destroy window
		SDL_DestroyWindow(window);

		// Quit SDL subsystems
		SDL_Quit();
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
	}

	return 0;
}