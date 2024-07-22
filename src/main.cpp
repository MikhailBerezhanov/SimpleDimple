#include <iostream>
#include <stdexcept>

#include "sdl.h"

#include "config.h"

#include "Logger.h"
#include "ErrorHandling.h"
#include "Window.h"

using namespace GameEngine;

int main(int argc, char *argv[])
{
    const LoggerInitializer loggerInitialer(LogLevel::DEBUG);

    try
    {
        AddLogHandler(CreateStdoutLogChannel());
        SetLogLevel(LogLevel::TRACE);

        LOG_TRACE("Starting " << argv[0]);

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
        }

        // Create window
        auto win = GameEngine::Window("SDL2 Window", GameEngine::Size2D{1000, 1000});

        std::string logo_file = std::string(ASSETS_IMAGES_DIR) + "/sdl_logo.bmp";

        // create texture from bmp
        const auto tex_id = win.AppendTexture(logo_file);
        const auto tex_id_2 = win.AppendTexture(logo_file);
        // obtain texture
        auto &tex = win.GetTexture(tex_id);
        auto &tex2 = win.GetTexture(tex_id_2);
        // resize texture
        tex.Downscale(6);
        tex2.Downscale(3);
        // make texture active
        win.SetTextureActive(tex_id, true);
        win.SetTextureActive(tex_id_2, true);
        // obtain texture rect to control its position
        auto dest = tex.GetRect();
        // sets initial x-position of object
        dest.x = (1000 - dest.w) / 2;
        // sets initial y-position of object
        dest.y = (1000 - dest.h) / 2;

        // speed of box
        int speed = 300;
        double angle = 0.0;

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
            // Calculate new coordinates for the structure
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

            // clear the screen
            win.Clear();
            // set texture position
            tex.SetPosition(GameEngine::Pos2D{dest.x, dest.y});
            tex.SetAngle(angle);
            // refresh
            win.Update();
            // present
            win.Present();
            // calculates to 60 fps
            SDL_Delay(1000 / 60);

            angle += 0.2;
        }

        std::cout << "End of loop!" << std::endl;

        // Quit SDL subsystems
        SDL_Quit();
    }
    catch (const std::exception &e)
    {
        HANDLE_EXCEPTION();
    }
    catch (...)
    {
        std::cerr << "unexpected exception occured" << std::endl;
    }

    return 0;
}