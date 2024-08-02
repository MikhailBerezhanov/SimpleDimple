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
    Size2D m_boundaries = {};
    const int m_speed = 5;
    int m_direction_x = 0;
    int m_direction_y = 0;
    // components
    std::shared_ptr<RendererComponent> m_renderer = nullptr;
    std::shared_ptr<TextureComponent> m_texture = nullptr;
    std::shared_ptr<TransformComponent> m_transform = nullptr;
public:
    using GameObject::GameObject;
    DemoGameObject(std::string name, const Size2D &boundaries)
        : GameObject(std::move(name))
        {
            m_boundaries = boundaries;
        }

    void Awake() override {

        LOG_DEBUG("Awake() called");
        // initialize members
        m_renderer = GetRenderer();
        m_texture = GetTexture();
        m_transform = GetTransform();

        // set transform size according to texture's initial size
        m_transform->Resize(m_texture->GetSize());
        // resize rect
        m_transform->Downscale(6);

        // get transform rectangle
        auto dest = m_transform->GetRect();
        // sets initial x-position of object
        dest.x = (m_boundaries.w - dest.w) / 2;
        // sets initial y-position of object
        dest.y = (m_boundaries.h - dest.h) / 2;
        // set object position (absolute)
        m_transform->SetPosition(Pos2D{dest.x, dest.y});
    }

    void OnEnable() override {
        LOG_DEBUG("OnEnable() called");
    }

    // IGameObject
    void OnUpdate() override
    {
        auto dest = m_transform->GetRect();

        // Apply movement based on speed and direction
        dest.x += m_speed * m_direction_x;
        dest.y += m_speed * m_direction_y;

        // Check x boundaries
        dest.x = std::max(0, std::min(dest.x, m_boundaries.w - dest.w));
        // Check y boundaries
        dest.y = std::max(0, std::min(dest.y, m_boundaries.h - dest.h));

        m_transform->SetPosition(Pos2D{dest.x, dest.y});
        m_transform->Rotate(0.2);

        m_renderer->AddTexture(m_texture);
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
    const auto winSize = Size2D{1000, 1000};

    try
    {
        AddLogHandler(CreateStdoutLogChannel());
        SetLogLevel(LogLevel::TRACE);

        LOG_TRACE("Starting " << argv[0]);

        const auto gameLoop = std::make_unique<GameLoop>();

        // Create window
        const auto mainWindow = std::make_shared<Window>("SDL2 Window", winSize);

        const auto logo_file = std::string(ASSETS_IMAGES_DIR) + "/sdl_logo.bmp";

        // create game object and set properties
        const auto go = std::make_shared<DemoGameObject>("logo", winSize);
        go->AddComponent(GameObjectComponentType::TRANSFORM);
        go->AddComponent(GameObjectComponentType::RENDERER, mainWindow->GetRenderContext());
        go->AddComponent(GameObjectComponentType::TEXTURE, logo_file);

        // add object to window and make active
        mainWindow->AppendObject(go, true);

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