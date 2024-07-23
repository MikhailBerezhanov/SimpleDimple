//
// Created by andrey on 14.07.24.
//

#pragma once

#include "Types.h"

namespace GameEngine {

    class IGameObjectComponent {
    public:
        virtual ~IGameObjectComponent() = default;
        virtual void OnUpdate() = 0;
//        virtual std::unique_ptr<IGameObjectComponent> Clone() const = 0;
    };
}
