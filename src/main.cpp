//mingw-specific
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

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "SDL Tutorial", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			// triggers the program that controls
    		// your graphics hardware and sets flags
			auto render_flags = SDL_RENDERER_ACCELERATED;
			// create renderer for images
			auto rend = SDL_CreateRenderer(window, -1, render_flags);

			//Get window surface
			auto screenSurface = SDL_GetWindowSurface( window );

			//Fill the surface white
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
			
			//Update the surface
			SDL_UpdateWindowSurface( window );
            
            //Hack to get window to stay up
            SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
		}
	}

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}