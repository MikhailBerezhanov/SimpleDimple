
#pragma once

#include "IGameObjectComponent.h"
#include <memory>
#include <vector>
#include <span>

namespace GameEngine {

    // ensure COMPONENT is derived from IGameObjectComponent
    template<typename T>
    concept COMPONENT = std::is_base_of_v<IGameObjectComponent, T>;

    template<COMPONENT C>
    class ComponentMatrix : public IGameObjectComponent {
    private:
        struct RowIdxLen {
            size_t start = 0;
            size_t len = 0;
        };
        using ComponentPtr = std::shared_ptr<C>;
        std::vector<ComponentPtr> m_matrix;
        std::vector<RowIdxLen> m_rowMap;

        void add_row(std::vector<ComponentPtr> &&row) {
            if (!row.empty()) {
                if (m_rowMap.empty()) {
                    m_rowMap.emplace_back(RowIdxLen{0, row.size()});
                } else {
                    const auto &prev_row = m_rowMap.back();
                    m_rowMap.emplace_back(RowIdxLen{prev_row.start + prev_row.len, row.size()});
                }
                m_matrix.insert(m_matrix.end(),
                                std::make_move_iterator(row.begin()),
                                std::make_move_iterator(row.end()));
            }
        }
    protected:
        explicit ComponentMatrix(std::vector<std::vector<ComponentPtr>> &&matrix) {
            for(auto && row : matrix) {
                add_row(std::move(row));
            }
        }
        friend class GameObject;
    public:
        ComponentMatrix(const ComponentMatrix&) = delete;
        ComponentMatrix &operator=(const ComponentMatrix&) = delete;
        ComponentMatrix(ComponentMatrix&&) = delete;
        ComponentMatrix &operator=(ComponentMatrix&&) = delete;
        ~ComponentMatrix() = default;

        size_t GetRowsNum() const {
            return m_rowMap.size();
        }
        std::span<const ComponentPtr> GetRow(size_t row_idx) const {
            const auto &rowIdxLen = m_rowMap.at(row_idx);
            return {m_matrix.data() + rowIdxLen.start, rowIdxLen.len};
        }
        const std::vector<ComponentPtr> &GetSerializedMatrix() const {
            return m_matrix;
        }

        void OnUpdate() override {};

    };

} // GameEngine