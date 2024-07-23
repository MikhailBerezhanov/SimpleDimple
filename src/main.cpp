#include <iostream>
#include <stdexcept>

#include "sdl.h"

#include "config.h"

#include "Logger.h"
#include "ErrorHandling.h"
#include "Window.h"
#include "GameObject.h"

#include "TransformComponent.h"
#include "RendererComponent.h"

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

        // create game object and set properties
        auto go = std::make_unique<GameObject>("logo");
        go->AddComponent(GameObjectComponentType::TRANSFORM);
        go->AddComponent(GameObjectComponentType::RENDERER, win.GetRenderContext());
        go->AddComponent(GameObjectComponentType::TEXTURE, logo_file);

        // get renderer
        auto renderer = std::dynamic_pointer_cast<RendererComponent>(go->GetComponent(GameObjectComponentType::RENDERER));
        // get texture
        auto texture = std::dynamic_pointer_cast<TextureComponent>(go->GetComponent(GameObjectComponentType::TEXTURE));
        // get transform
        auto transform = std::dynamic_pointer_cast<TransformComponent>(go->GetComponent(GameObjectComponentType::TRANSFORM));

        // set transform size according to texture's initial size
        transform->Resize(texture->GetSize());
        // resize rect
        transform->Downscale(6);

        // add object to window and make active
        auto go_id = win.AppendObject(std::move(go));
        win.SetObjectActive(go_id, true);

        // get transform rectangle
        auto dest = transform->GetRect();

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
            //todo: these should be set inside GameObject's OnUpdate()

            transform->SetPosition(Pos2D{dest.x, dest.y}); // set object position (absolute)
            //transform->SetAngle(angle); // set object rotation angle (absolute)
            transform->Rotate(0.2); // set object rotation angle (relative)
            renderer->AddTexture(texture); // set renderer active texture

            // update (calls go->renderer->OnUpdate())
            win.Update();
            // present
            win.Present();
            // calculates to 60 fps
            SDL_Delay(1000 / 60);
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
        std::cerr << "unexpected exception occurred" << std::endl;
    }

    return 0;
}