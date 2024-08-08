
#include "GameObject.h"
#include "ErrorHandling.h"


namespace GameEngine {

    GameObject::GameObject(std::string name)
        :m_name(std::move(name))
        {}

    void GameObject::AddComponent(GameObjectComponentType type) {
        AddComponent(type, std::any()); // call with default no-value
    }

    void GameObject::add_transform(const std::any &arg) {
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
        m_components[GameObjectComponentType::TRANSFORM] = new_transform;
    }

    void GameObject::add_renderer(const std::any &arg) {
        EXPECT_MSG(arg.type() == typeid(RenderContext),
                   "Invalid argument type");
        // check if Transform exists
        const auto transform = GetComponent<const TransformComponent>();
        const auto context = std::any_cast<RenderContext>(arg);
        m_components[GameObjectComponentType::RENDERER] =
                std::shared_ptr<RendererComponent>(new RendererComponent(context, transform));
    }

    void GameObject::add_texture(const std::any &arg) {
        EXPECT_MSG(arg.type() == typeid(std::string) || arg.type() == typeid(Size2D),
                   "Invalid argument type");
        std::shared_ptr<TextureComponent> new_texture;
        // check if Renderer exists
        const auto renderer = GetComponent<const RendererComponent>();
        if (arg.type() == typeid(std::string)) {
            const auto name = std::any_cast<std::string>(arg);
            new_texture.reset(new TextureComponent(renderer->GetRenderContext(), name));
        }else {
            const auto size = std::any_cast<Size2D>(arg);
            new_texture.reset(new TextureComponent(renderer->GetRenderContext(), size));
        }
        m_components[GameObjectComponentType::TEXTURE] = new_texture;
    }

    void GameObject::add_texture_matrix(const std::any &arg) {
        using tex_files_list_type = std::initializer_list<std::initializer_list<std::string>>;
        EXPECT_MSG(arg.type() == typeid(tex_files_list_type),
                   "Invalid argument type");
        // check if Renderer exists
        const auto renderer = GetComponent<const RendererComponent>();
        const auto tex_files = std::any_cast<tex_files_list_type>(arg);
        // populate with textures
        std::vector<std::vector<std::shared_ptr<TextureComponent>>> rows;
        for (const auto &row : tex_files) {
            std::vector<std::shared_ptr<TextureComponent>> tex_row;
            for (const auto &filename : row) {
                tex_row.emplace_back(new TextureComponent(renderer->GetRenderContext(), filename));
            }
            rows.emplace_back(std::move(tex_row));
        }
        m_components[GameObjectComponentType::TEXTURE_MATRIX] =
                std::shared_ptr<TextureMatrixComponent>(new TextureMatrixComponent(std::move(rows)));
    }

    void GameObject::AddComponent(GameObjectComponentType type, std::any arg) {

        EXPECT_MSG(m_components.find(type) == m_components.end(),
                   "Component " << static_cast<unsigned int>(type) << " already exists");

        switch (type) {

            case GameObjectComponentType::TRANSFORM: {
                add_transform(arg);
                break;
            }
            case GameObjectComponentType::RENDERER: {
                add_renderer(arg);
                break;
            }
            case GameObjectComponentType::TEXTURE: {
                add_texture(arg);
                break;
            }
            case GameObjectComponentType::TEXTURE_MATRIX: {
                add_texture_matrix(arg);
                break;
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