
#include "TextureMatrixComponent.h"

namespace GameEngine {


    TextureMatrixComponent::TextureMatrixComponent()
    {}

    void TextureMatrixComponent::AddRow(std::vector<TexturePtr> row) {
        if (m_rowMap.empty()) {
            m_rowMap.emplace_back(0, row.size());
        } else {
            const auto &prev_row = m_rowMap.back();
            m_rowMap.emplace_back(prev_row.start + prev_row.len, row.size());
        }
        m_textureMatrix.insert(m_textureMatrix.end(),
                               std::make_move_iterator(row.begin()),
                               std::make_move_iterator(row.end()));
    }

    void TextureMatrixComponent::Clear() {
        m_textureMatrix.clear();
        m_rowMap.clear();
    }

    size_t TextureMatrixComponent::GetRowsNum() const {
        return m_rowMap.size();
    }

    std::span<const TextureMatrixComponent::TexturePtr>
            TextureMatrixComponent::GetRow(size_t row_idx) const {

        const auto &row = m_rowMap.at(row_idx);
        return {m_textureMatrix.data() + row.start, row.len};
    }


} // GameEngine