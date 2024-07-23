//
// Created by andrey on 14.07.24.
//

#pragma once

#include <any>

#include "IGameObjectComponent.h"

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
        //todo: OnKeyDown()
        //todo: OnKeyUp()
//        virtual std::unique_ptr<IGameObject> Clone() const = 0;
    };

} // GameEngine