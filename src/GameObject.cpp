//
// Created by andrey on 14.07.24.
//

#include "GameObject.h"
#include "TransformComponent.h"
#include "RendererComponent.h"
#include "TextureComponent.h"

namespace GameEngine {

    GameObject::GameObject(std::string name)
        :m_name(std::move(name))
        {}

    void GameObject::AddComponent(GameObjectComponentType type) {
        AddComponent(type, None{});
    }

    void GameObject::AddComponent(GameObjectComponentType type, std::any arg) {

        if (m_components.find(type) != m_components.end()) {
            throw std::runtime_error(std::string(GameObjectComponentNames[static_cast<unsigned int>(type)]) + " already exists");
        }

        switch (type) {

            case GameObjectComponentType::TRANSFORM: {
                TransformComponent *new_transform;
                if (arg.type() != typeid(None)) {
                    const auto pos = std::any_cast<Size2D>(arg);
                    new_transform = new TransformComponent(pos);
                }
                else {
                    new_transform = new TransformComponent();
                }
                m_components[type] = std::shared_ptr<TransformComponent>(new_transform);
                break;
            }
            case GameObjectComponentType::RENDERER: {
                // check if Transform exists
                const auto &transform_it = m_components.find(GameObjectComponentType::TRANSFORM);
                if (transform_it == m_components.end()) {
                    throw std::runtime_error("Renderer requires Transform component");
                }
                const auto context = std::any_cast<RenderContext>(arg);
                const auto transform = std::dynamic_pointer_cast<const TransformComponent>(transform_it->second);
                m_components[type] = std::shared_ptr<RendererComponent>(new RendererComponent(context, transform));
                break;
            }
            case GameObjectComponentType::TEXTURE: {
                // check if Renderer exists
                const auto &renderer_it = m_components.find(GameObjectComponentType::RENDERER);
                if (renderer_it == m_components.end()) {
                    throw std::runtime_error("Texture requires Renderer component");
                }
                std::shared_ptr<TextureComponent> new_texture;
                const auto renderer = std::dynamic_pointer_cast<const RendererComponent>(renderer_it->second);
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
            default:
                throw std::runtime_error("No handler for component type " + std::to_string(static_cast<unsigned int>(type)));
        }
    }

    void GameObject::OnUpdate() {
        // do nothing, should be overridden in derived classes
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