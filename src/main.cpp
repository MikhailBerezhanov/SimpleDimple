#include <iostream>
#include <stdexcept>

#include "sdl.h"

#include "config.h"

#include "sdl_window.h"
#include "sdl_surface.h"

// Screen dimension constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

int main(int argc, char *args[])
{

    try
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
        }

        GameEngine::Window win("SDL2 Window", SCREEN_WIDTH, SCREEN_HEIGHT);
        auto rend = win.create_renderer();

        std::cout << "Created renderer" << std::endl;

        std::string logo_file = std::string(ASSETS_IMAGES_DIR) + "/sdl_logo.bmp";

        // load bmp as surface
        GameEngine::Surface surface(logo_file);
        // create texture from surface
        auto tex = rend->create_texture(surface);

        // connect texture with dest to control position
        GameEngine::Rect dest;
        auto [format, access, w, h] = tex->query();
        dest.w = w;
        dest.h = h;

        // adjust height and width of image box
        dest.w /= 6;
        dest.h /= 6;

        // sets initial x-position of object
        dest.x = (1000 - dest.w) / 2;

        // sets initial y-position of object
        dest.y = (1000 - dest.h) / 2;

        // speed of box
        int speed = 300;

        SDL_Event event;
        bool quit = false;
        while (!quit)
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                    {
                        quit = true;
                        break;
                    }
                    case SDL_KEYDOWN:
                    {
                        switch (event.key.keysym.scancode)
                        {
                            case SDL_SCANCODE_W:
                            case SDL_SCANCODE_UP:
                                dest.y -= speed / 30;
                                break;
                            case SDL_SCANCODE_A:
                            case SDL_SCANCODE_LEFT:
                                dest.x -= speed / 30;
                                break;
                            case SDL_SCANCODE_S:
                            case SDL_SCANCODE_DOWN:
                                dest.y += speed / 30;
                                break;
                            case SDL_SCANCODE_D:
                            case SDL_SCANCODE_RIGHT:
                                dest.x += speed / 30;
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
            // right boundary
            if (dest.x + dest.w > 1000)
                dest.x = 1000 - dest.w;

            // left boundary
            if (dest.x < 0)
                dest.x = 0;

            // bottom boundary
            if (dest.y + dest.h > 1000)
                dest.y = 1000 - dest.h;

            // upper boundary
            if (dest.y < 0)
                dest.y = 0;

            // clears the screen
            rend->clear();
            rend->copy(tex, nullptr, &dest);
            rend->present();

            // calculates to 60 fps
            SDL_Delay(1000 / 60);
        }

        std::cout << "End of loop!" << std::endl;

        // Quit SDL subsystems
        SDL_Quit();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}