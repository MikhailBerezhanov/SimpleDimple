#include <iostream>
#include <stdexcept>

#include "sdl.h"

#include "config.h"

#include "Logger.h"
#include "ErrorHandling.h"
#include "Window.h"
#include "GameObject.h"
#include "GameLoop.h"

#include "TransformComponent.h"
#include "RendererComponent.h"

using namespace GameEngine;


class DemoGameObject : public GameObject, public IInputEventSubscriber
{
private:
    int m_speed = 300;

public:
    using GameObject::GameObject;

    // IGameObject
    void OnUpdate() override
    {
        const auto renderer = GetRenderer();
        const auto texture = GetTexture();
        const auto transform = GetTransform();

        auto dest = transform->GetRect();

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

        transform->SetPosition(Pos2D{dest.x, dest.y});
        transform->Rotate(0.2);
        renderer->AddTexture(texture);
    }

    // IInputEventSubscriber
    void OnKeyUp(KeyCodes keyCode) override {}

    void OnKeyDown(KeyCodes keyCode) override
    {
        const auto transform = GetTransform();
        auto dest = transform->GetRect();

        switch (keyCode)
        {
            case KeyCodes::W:
            case KeyCodes::ARROW_UP:
                dest.y -= m_speed / 30;
                break;
            case KeyCodes::A:
            case KeyCodes::ARROW_LEFT:
                dest.x -= m_speed / 30;
                break;
            case KeyCodes::S:
            case KeyCodes::ARROW_DOWN:
                dest.y += m_speed / 30;
                break;
            case KeyCodes::D:
            case KeyCodes::ARROW_RIGHT:
                dest.x += m_speed / 30;
                break;
            default:
                break;
        }

        transform->SetPosition(Pos2D{dest.x, dest.y});
    }
};

int main(int argc, char *argv[])
{
    const LoggerInitializer loggerInitialer(LogLevel::DEBUG);

    try
    {
        AddLogHandler(CreateStdoutLogChannel());
        SetLogLevel(LogLevel::TRACE);

        LOG_TRACE("Starting " << argv[0]);

        const auto gameLoop = std::make_unique<GameLoop>();

        // Create window
        const auto mainWindow = std::make_shared<Window>("SDL2 Window", Size2D{1000, 1000});

        std::string logo_file = std::string(ASSETS_IMAGES_DIR) + "/sdl_logo.bmp";

        // create game object and set properties
        auto go = std::make_shared<DemoGameObject>("logo");
        go->AddComponent(GameObjectComponentType::TRANSFORM);
        go->AddComponent(GameObjectComponentType::RENDERER, mainWindow->GetRenderContext());
        go->AddComponent(GameObjectComponentType::TEXTURE, logo_file);

        // get renderer
        auto renderer = go->GetRenderer();
        // get texture
        auto texture = go->GetTexture();
        // get transform
        auto transform = go->GetTransform();

        // set transform size according to texture's initial size
        transform->Resize(texture->GetSize());
        // resize rect
        transform->Downscale(6);

        // add object to window and make active
        auto go_id = mainWindow->AppendObject(go, true);

        // get transform rectangle
        auto dest = transform->GetRect();

        // sets initial x-position of object
        dest.x = (1000 - dest.w) / 2;
        // sets initial y-position of object
        dest.y = (1000 - dest.h) / 2;

        // speed of box
        int speed = 300;

        // set object position (absolute)
        transform->SetPosition(Pos2D{dest.x, dest.y}); 
         // set renderer active texture
        renderer->AddTexture(texture);

        gameLoop->SubscribeToInputEvents(go);
        gameLoop->SetWindow(mainWindow);
        gameLoop->Run();
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