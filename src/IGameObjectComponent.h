
#pragma once

#include "Types.h"

namespace GameEngine {

    class IGameObjectComponent {
    public:
        virtual ~IGameObjectComponent() = default;
        virtual void OnUpdate() = 0;
    };
}
