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

struct TestMatrix : public ComponentMatrix<Dummy> {
    explicit TestMatrix(std::vector<std::vector<std::shared_ptr<Dummy>>> && vec)
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
    TestMatrix matrix({
        {},
        {std::make_shared<Dummy>()},
        {}
    });
    ASSERT_EQ(matrix.GetRowsNum(), 1);
}

MATRIX_TEST(CheckRange) {
    auto d = std::make_shared<Dummy>();
    TestMatrix matrix({
        {d},
        {d}
    });
    ASSERT_NO_THROW(matrix.GetRow(0));
    ASSERT_NO_THROW(matrix.GetRow(1));
    ASSERT_THROW(matrix.GetRow(2), std::out_of_range);
}

MATRIX_TEST(CheckRows) {
    auto d = std::make_shared<Dummy>();
    TestMatrix matrix({
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
    TestMatrix matrix(generate_dummies(3, 5));
    int cnt = 0;
    for (int i = 0; i < 15; ++i) {
        ASSERT_EQ(matrix.GetRow(i/5)[i%5]->m_int, cnt);
        ++cnt;
    }
}

MATRIX_TEST(CheckSerialized) {
    TestMatrix matrix(generate_dummies(3, 5));
    auto serialized = matrix.GetSerializedMatrix();
    int cnt = 0;
    for (const auto & d : serialized) {
        ASSERT_EQ(d->m_int, cnt);
        ++cnt;
    }
}


