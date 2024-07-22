#include <TransformComponent.h>
#include <gtest/gtest.h>

#define FIXTURE TransformComponentTest
#define TRANSFORM_TEST(NAME) TEST_F(FIXTURE, NAME)

using namespace GameEngine;

// test fixture
class FIXTURE : public testing::Test {
private:
    // derived transform class
    class TransformTest : public TransformComponent {
    public:
        explicit TransformTest(const Size2D &size)
                : TransformComponent(size) {}
    };
protected:
    TransformTest m_transform;
    FIXTURE()
        : m_transform({}) {}
};

TRANSFORM_TEST(CheckPosition) {
    m_transform.SetPosition({-12,12});
    const auto pos = m_transform.GetPosition();
    ASSERT_TRUE(pos.x == -12 && pos.y == 12);
}

TRANSFORM_TEST(CheckSize) {
    m_transform.Resize({10,10});
    const auto size = m_transform.GetSize();
    ASSERT_TRUE(size.w == 10 && size.h == 10);
}

TRANSFORM_TEST(CheckRect) {
    m_transform.SetPosition({-12,12});
    m_transform.Resize({10,10});
    const auto rect = m_transform.GetRect();
    ASSERT_TRUE(rect.x == -12 && rect.y == 12 && rect.w == 10 && rect.h == 10);
}
