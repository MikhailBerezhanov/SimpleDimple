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
    const int m_speed = 5;
    const int m_boundary_x = 1000;
    const int m_boundary_y = 1000;
    int m_direction_x = 0;
    int m_direction_y = 0;
public:
    using GameObject::GameObject;

    // IGameObject
    void OnUpdate() override
    {
        const auto renderer = GetRenderer();
        const auto texture = GetTexture();
        const auto transform = GetTransform();

        auto dest = transform->GetRect();

        // Apply movement based on speed and direction
        dest.x += m_speed * m_direction_x;
        dest.y += m_speed * m_direction_y;

        // Check x boundaries
        dest.x = std::max(0, std::min(dest.x, m_boundary_x - dest.w));
        // Check y boundaries
        dest.y = std::max(0, std::min(dest.y, m_boundary_y - dest.h));

        transform->SetPosition(Pos2D{dest.x, dest.y});
        transform->Rotate(0.2);
        renderer->AddTexture(texture);
    }

    // IInputEventSubscriber
    void OnKeyUp(KeyCodes keyCode) override
    {
        switch (keyCode) {
            case KeyCodes::W:
            case KeyCodes::S:
            case KeyCodes::ARROW_UP:
            case KeyCodes::ARROW_DOWN:
                m_direction_y = 0;
                break;
            case KeyCodes::A:
            case KeyCodes::D:
            case KeyCodes::ARROW_LEFT:
            case KeyCodes::ARROW_RIGHT:
                m_direction_x = 0;
                break;
            default:
                break;
        }
    }

    void OnKeyDown(KeyCodes keyCode) override
    {
        switch (keyCode) {
            case KeyCodes::W:
            case KeyCodes::ARROW_UP:
                m_direction_y = -1;
                break;
            case KeyCodes::A:
            case KeyCodes::ARROW_LEFT:
                m_direction_x = -1;
                break;
            case KeyCodes::S:
            case KeyCodes::ARROW_DOWN:
                m_direction_y = 1;
                break;
            case KeyCodes::D:
            case KeyCodes::ARROW_RIGHT:
                m_direction_x = 1;
                break;
            default:
                break;
        }
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

        const auto logo_file = std::string(ASSETS_IMAGES_DIR) + "/sdl_logo.bmp";

        // create game object and set properties
        const auto go = std::make_shared<DemoGameObject>("logo");
        go->AddComponent(GameObjectComponentType::TRANSFORM);
        go->AddComponent(GameObjectComponentType::RENDERER, mainWindow->GetRenderContext());
        go->AddComponent(GameObjectComponentType::TEXTURE, logo_file);

        // get renderer
        const auto renderer = go->GetRenderer();
        // get texture
        const auto texture = go->GetTexture();
        // get transform
        const auto transform = go->GetTransform();

        // set transform size according to texture's initial size
        transform->Resize(texture->GetSize());
        // resize rect
        transform->Downscale(6);

        // add object to window and make active
        const auto go_id = mainWindow->AppendObject(go, true);

        // get transform rectangle
        auto dest = transform->GetRect();
        // sets initial x-position of object
        dest.x = (1000 - dest.w) / 2;
        // sets initial y-position of object
        dest.y = (1000 - dest.h) / 2;
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