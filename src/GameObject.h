
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
    protected:
        void OnUpdate() override;
    public:
        explicit GameObject(std::string name);
        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        ~GameObject() override = default;
        // todo: Clone()
        void AddComponent(GameObjectComponentType type) final; // cannot be overridden
        void AddComponent(GameObjectComponentType type, std::any arg) final; // cannot be overridden
        std::shared_ptr<IGameObjectComponent> GetComponent(GameObjectComponentType type) const override;

    };

} // GameEngine