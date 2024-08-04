
#pragma once

#include "IGameObjectComponent.h"
#include "GameObjectComponentTypes.h"

#include <any>
#include <memory>

namespace GameEngine {

    class IGameObject {
    public:
        virtual ~IGameObject() = default;

        virtual void AddComponent(GameObjectComponentType type) = 0;
        virtual void AddComponent(GameObjectComponentType type, std::any arg) = 0;
        virtual std::shared_ptr<IGameObjectComponent> GetComponent(GameObjectComponentType type) const = 0;
        //todo: AddChild()
        virtual void OnUpdate() = 0;
        virtual void Update() = 0;
        virtual void Awake() = 0; // call once when instantiated
        virtual void OnEnable() = 0;
        virtual void OnDisable() = 0;
    };

} // GameEngine