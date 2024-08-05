
#include "GameObject.h"
#include "ErrorHandling.h"


namespace GameEngine {

    GameObject::GameObject(std::string name)
        :m_name(std::move(name))
        {}

    void GameObject::AddComponent(GameObjectComponentType type) {
        AddComponent(type, std::any()); // call with default no-value
    }

    void GameObject::AddComponent(GameObjectComponentType type, std::any arg) {

        EXPECT_MSG(m_components.find(type) == m_components.end(),
                   static_cast<unsigned int>(type) << " already exists");

        switch (type) {

            case GameObjectComponentType::TRANSFORM: {
                EXPECT_MSG(!arg.has_value() || arg.type() == typeid(Size2D),
                           "Invalid argument type");
                std::shared_ptr<TransformComponent> new_transform;
                if (arg.has_value()) {
                    const auto pos = std::any_cast<Size2D>(arg);
                    new_transform.reset(new TransformComponent(pos));
                }
                else {
                    new_transform.reset(new TransformComponent());
                }
                m_components[type] = new_transform;
                break;
            }
            case GameObjectComponentType::RENDERER: {
                EXPECT_MSG(arg.type() == typeid(RenderContext),
                           "Invalid argument type");
                // check if Transform exists
                const auto transform = GetComponent<const TransformComponent>();
                const auto context = std::any_cast<RenderContext>(arg);
                m_components[type] = std::shared_ptr<RendererComponent>(new RendererComponent(context, transform));
                break;
            }
            case GameObjectComponentType::TEXTURE: {
                EXPECT_MSG(arg.type() == typeid(std::string) || arg.type() == typeid(Size2D),
                           "Invalid argument type");
                std::shared_ptr<TextureComponent> new_texture;
                // check if Renderer exists
                const auto renderer = GetComponent<const RendererComponent>();
                const auto context = renderer->GetRenderContext();
                if (arg.type() == typeid(std::string)) {
                    const auto name = std::any_cast<std::string>(arg);
                    new_texture.reset(new TextureComponent(context, name));
                }else {
                    const auto size = std::any_cast<Size2D>(arg);
                    new_texture.reset(new TextureComponent(context, size));
                }
                m_components[type] = new_texture;
                break;
            }
            case GameObjectComponentType::TEXTURE_MATRIX: {
                using tex_files_list_type = std::initializer_list<std::string>;
                EXPECT_MSG(arg.type() == typeid(tex_files_list_type),
                           "Invalid argument type");
                // check if Renderer exists
                const auto renderer = GetComponent<const RendererComponent>();
                const auto context = renderer->GetRenderContext();
                const auto tex_files = std::any_cast<tex_files_list_type>(arg);
                auto texture_matrix = std::shared_ptr<TextureMatrixComponent>(new TextureMatrixComponent());
                // populate with textures

            }
            default:
                EXPECT_MSG(false, "No handler for component type " << static_cast<unsigned int>(type));
        }
    }

    std::shared_ptr<IGameObjectComponent> GameObject::GetComponent(GameObjectComponentType type) const {
        EXPECT(m_components.find(type) != m_components.end());
        return m_components.at(type);
    }

    void GameObject::Update() {
        // call OnUpdate()
        this->OnUpdate();
        // call OnUpdate for all components
        for(const auto &[key, val] : m_components) {
            val->OnUpdate();
        }
        //todo: children?
    }
    
} // GameEngine