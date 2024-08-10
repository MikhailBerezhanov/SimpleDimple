#include <ComponentMatrix.h>
#include <gtest/gtest.h>

#include <memory>

#define MATRIX_TEST(name) TEST(MatrixTest, name)

using namespace GameEngine;

struct Dummy : public IGameObjectComponent {
    int m_int;
    explicit Dummy(int i = 0)
        : m_int(i) {}
    ~Dummy() = default;
    void OnUpdate() override {}
};

struct MatrixMock : public ComponentMatrix<Dummy> {
    explicit MatrixMock(std::vector<std::vector<std::shared_ptr<Dummy>>> && vec)
        : ComponentMatrix<Dummy>(std::move(vec))
    {}
};

auto generate_dummies(int rows, int cols) {
    std::vector<std::vector<std::shared_ptr<Dummy>>> vec;
    int cnt = 0;
    for(int r = 0; r < rows; ++r) {
        std::vector<std::shared_ptr<Dummy>> row;
        row.reserve(cols);
        for(int c = 0; c < cols; ++c) {
            row.emplace_back(std::make_shared<Dummy>(cnt++));
        }
        vec.emplace_back(std::move(row));
    }
    return vec;
}

MATRIX_TEST(CheckIgnoreEmpty) {
    MatrixMock matrix({
        {},
        {std::make_shared<Dummy>()},
        {}
    });
    ASSERT_EQ(matrix.GetRowsNum(), 1);
}

MATRIX_TEST(CheckRange) {
    auto d = std::make_shared<Dummy>();
    MatrixMock matrix({
        {d},
        {d}
    });
    ASSERT_NO_THROW(matrix.GetRow(0));
    ASSERT_NO_THROW(matrix.GetRow(1));
    ASSERT_THROW(matrix.GetRow(2), std::out_of_range);
}

MATRIX_TEST(CheckRows) {
    auto d = std::make_shared<Dummy>();
    MatrixMock matrix({
        {d, d, d},
        {d, d},
        {d, d, d, d},
        {d}
    });
    ASSERT_EQ(matrix.GetRowsNum(), 4);
    ASSERT_EQ(matrix.GetRow(0).size(), 3);
    ASSERT_EQ(matrix.GetRow(1).size(), 2);
    ASSERT_EQ(matrix.GetRow(2).size(), 4);
    ASSERT_EQ(matrix.GetRow(3).size(), 1);
}

MATRIX_TEST(CheckSpan) {
    MatrixMock matrix(generate_dummies(3, 5));
    ASSERT_EQ(matrix.GetRow(0)[0]->m_int, 0);
    ASSERT_EQ(matrix.GetRow(0)[4]->m_int, 4);
    ASSERT_EQ(matrix.GetRow(1)[0]->m_int, 5);
    ASSERT_EQ(matrix.GetRow(1)[4]->m_int, 9);
    ASSERT_EQ(matrix.GetRow(2)[0]->m_int, 10);
    ASSERT_EQ(matrix.GetRow(2)[4]->m_int, 14);
}

MATRIX_TEST(CheckSerialized) {
    MatrixMock matrix(generate_dummies(3, 5));
    auto serialized = matrix.GetSerializedMatrix();
    int cnt = 0;
    for (const auto & d : serialized) {
        ASSERT_EQ(d->m_int, cnt);
        ++cnt;
    }
}


