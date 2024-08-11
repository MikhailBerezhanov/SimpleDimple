
#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "IGameObject.h"

namespace GameEngine {

    class GameObject : public IGameObject {
    private:
        using ComponentPtr = std::shared_ptr<IGameObjectComponent>;
        std::string m_name;
        std::map<GameObjectComponentType, ComponentPtr> m_components;
        void Update() final; // cannot be overridden
        void add_transform(const std::any &arg);
        void add_renderer(const std::any &arg);
        void add_texture(const std::any &arg);
        void add_texture_matrix(const std::any &arg);
    protected:
        void OnUpdate() override {};
        void Awake() override {};
        void OnEnable() override {};
        void OnDisable() override {};
    public:
        explicit GameObject(std::string name);
        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject &&) = delete;
        GameObject &operator=(GameObject &&) = delete;
        ~GameObject() override = default;

        void AddComponent(GameObjectComponentType type) final; // cannot be overridden
        void AddComponent(GameObjectComponentType type, std::any arg) final; // cannot be overridden
        std::shared_ptr<IGameObjectComponent> GetComponent(GameObjectComponentType type) const override;

        /// template GetComponent
        template<typename T>
        std::shared_ptr<T> GetComponent() const {
            return std::dynamic_pointer_cast<T>(
                    GetComponent(
                            // remove cv qualifiers to correctly find the type
                            ComponentTypes<std::remove_cvref_t<T>>::type
                            )
                    );
        }

    };

} // GameEngine