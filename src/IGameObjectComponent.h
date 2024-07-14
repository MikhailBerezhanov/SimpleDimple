//
// Created by andrey on 14.07.24.
//

#pragma once

#include "Types.h"

namespace GameEngine {
    class IGameObjectComponent {
    public:
        virtual void OnUpdate() = 0;
    };
}
