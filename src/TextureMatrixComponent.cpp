
#include "TextureMatrixComponent.h"

namespace GameEngine {

    TextureMatrixComponent::TextureMatrixComponent(std::vector<std::vector<TexturePtr>> &&rows) {
        for(auto & row : rows) {
            add_row(std::move(row));
        }
    }

    void TextureMatrixComponent::add_row(std::vector<TexturePtr> &&row) {
        if (m_rowMap.empty()) {
            m_rowMap.emplace_back(RowIdxLen{0, row.size()});
        } else {
            const auto &prev_row = m_rowMap.back();
            m_rowMap.emplace_back(RowIdxLen{prev_row.start + prev_row.len, row.size()});
        }
        m_textureMatrix.insert(m_textureMatrix.end(),
                               std::make_move_iterator(row.begin()),
                               std::make_move_iterator(row.end()));
    }

    size_t TextureMatrixComponent::GetRowsNum() const {
        return m_rowMap.size();
    }

    std::span<const TextureMatrixComponent::TexturePtr>
            TextureMatrixComponent::GetRow(size_t row_idx) const {

        const auto &row = m_rowMap.at(row_idx);
        return {m_textureMatrix.data() + row.start, row.len};
    }

    const std::vector<TextureMatrixComponent::TexturePtr> &
            TextureMatrixComponent::GetSerializedMatrix() const {
        return m_textureMatrix;
    }

} // GameEngine