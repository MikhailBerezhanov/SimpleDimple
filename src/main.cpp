#include <iostream>
#include <stdexcept>

#include "sdl.h"

#include "config.h"

#include <vector>
#include <span>

// Screen dimension constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

int main(int argc, char *args[])
{
    try
    {
        // test C++20
        std::cout << "CPP20 test: (should print `43` 100 times)\n";
        std::vector<int> vec(100, 43);
        std::span sp(vec.data(), vec.size());
        for (const auto i : sp)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        // ----


        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
        }
        auto window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (!window)
        {
            throw std::runtime_error("SDL_CreateWindow error! SDL_Error: " + std::string(SDL_GetError()));
        }

        auto rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!rend)
        {
            throw std::runtime_error("SDL_CreateRenderer error! SDL_Error: " + std::string(SDL_GetError()));
        }

        std::cout << "Created renderer" << std::endl;

        std::string logo_file = std::string(ASSETS_IMAGES_DIR) + "/sdl_logo.bmp";
        // load bmp
        auto logo_surface = IMG_Load(logo_file.c_str());

        // convert bmp to display format
        auto tex = SDL_CreateTextureFromSurface(rend, logo_surface);

        // clear mem
        SDL_FreeSurface(logo_surface);

        // connect texture with dest to control position
        SDL_Rect dest;
        SDL_QueryTexture(tex, nullptr, nullptr, &dest.w, &dest.h);

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
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, tex, NULL, &dest);

            // triggers the double buffers
            // for multiple rendering
            SDL_RenderPresent(rend);

            // calculates to 60 fps
            SDL_Delay(1000 / 60);
        }

        std::cout << "End of loop!" << std::endl;

        // destroy texture
        SDL_DestroyTexture(tex);

        // destroy renderer
        SDL_DestroyRenderer(rend);

        // Destroy window
        SDL_DestroyWindow(window);

        // Quit SDL subsystems
        SDL_Quit();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}