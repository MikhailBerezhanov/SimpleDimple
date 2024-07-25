
#pragma once

#include <vector>
#include <memory>
#include <span>

#include "IGameObjectComponent.h"
#include "TextureComponent.h"

namespace GameEngine {

    class TextureMatrixComponent : public IGameObjectComponent {
    private:
        struct RowIdxLen {
            unsigned int start = 0;
            unsigned int len = 0;
        };
        using TexturePtr = std::shared_ptr<TextureComponent>;
        std::vector<TexturePtr> m_textureMatrix;
        std::vector<RowIdxLen> m_rowMap;

        TextureMatrixComponent();
    public:


        void AddRow(std::vector<TexturePtr> row);
        void Clear();
        size_t GetRowsNum() const;
        std::span<const TexturePtr> GetRow(size_t row) const;
    };

} // GameEngine