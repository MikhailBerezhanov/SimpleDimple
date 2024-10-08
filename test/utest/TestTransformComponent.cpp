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
    ASSERT_EQ(pos.x, -12);
    ASSERT_EQ(pos.y, 12);
}

TRANSFORM_TEST(CheckSize) {
    m_transform.Resize({10,10});
    const auto size = m_transform.GetSize();
    ASSERT_EQ(size.w, 10);
    ASSERT_EQ(size.h, 10);
}

TRANSFORM_TEST(CheckRect) {
    m_transform.SetPosition({-12,12});
    m_transform.Resize({10,10});
    const auto rect = m_transform.GetRect();
    ASSERT_EQ(rect.x, -12);
    ASSERT_EQ(rect.y, 12);
    ASSERT_EQ(rect.w, 10);
    ASSERT_EQ(rect.h, 10);
}

TRANSFORM_TEST(DownscaleByZero) {
    ASSERT_ANY_THROW(m_transform.Downscale(0));
}

TRANSFORM_TEST(DownscaleByNegative) {
    ASSERT_ANY_THROW(m_transform.Downscale(-1));
}

TRANSFORM_TEST(UpscaleByNegative) {
    ASSERT_ANY_THROW(m_transform.Downscale(-1));
}

TRANSFORM_TEST(ResizeByNegative) {
    ASSERT_ANY_THROW(m_transform.Resize(
            Size2D{-1, -1}
            )
    );
}

TRANSFORM_TEST(CheckDownscale) {
    m_transform.Resize(Size2D{10,20});
    m_transform.Downscale(2);
    const auto size = m_transform.GetSize();
    ASSERT_EQ(size.w, 5);
    ASSERT_EQ(size.h, 10);
}

TRANSFORM_TEST(CheckDownscaleImperfect) {
    m_transform.Resize(Size2D{11,21});
    m_transform.Downscale(2);
    const auto size = m_transform.GetSize();
    ASSERT_EQ(size.w, 5);
    ASSERT_EQ(size.h, 10);
}

TRANSFORM_TEST(CheckUpscale) {
    m_transform.Resize(Size2D{10,20});
    m_transform.Upscale(2);
    const auto size = m_transform.GetSize();
    ASSERT_EQ(size.w, 20);
    ASSERT_EQ(size.h, 40);
}

TRANSFORM_TEST(CheckAngle) {
    m_transform.SetAngle(123);
    ASSERT_EQ(m_transform.GetAngle(), 123.0);
}

TRANSFORM_TEST(CheckNegativeAngle) {
    m_transform.SetAngle(-123);
    ASSERT_EQ(m_transform.GetAngle(), -123.0);
}

TRANSFORM_TEST(CheckAngleOverflow) {
    m_transform.SetAngle(450);
    ASSERT_EQ(m_transform.GetAngle(), 90.0);
}

TRANSFORM_TEST(CheckNegativeAngleOverflow) {
    m_transform.SetAngle(-450);
    ASSERT_EQ(m_transform.GetAngle(), -90.0);
}

TRANSFORM_TEST(CheckCenter) {
    m_transform.SetCenter(Pos2D{10, 0});
    const auto center = m_transform.GetCenter();
    ASSERT_EQ(center.x, 10);
    ASSERT_EQ(center.y, 0);
}

TRANSFORM_TEST(CheckCenterAfterResize) {
    m_transform.Resize(Size2D{10,10});
    const auto center = m_transform.GetCenter();
    // should be w/2 h/2
    ASSERT_EQ(center.x, 5);
    ASSERT_EQ(center.y, 5);
}

TRANSFORM_TEST(CheckCenterAfterDownscale) {
    m_transform.Resize(Size2D{10,10});
    m_transform.Downscale(2);
    const auto center = m_transform.GetCenter();
    ASSERT_EQ(center.x, 2);
    ASSERT_EQ(center.y, 2);
}

TRANSFORM_TEST(CheckCenterAfterUpscale) {
    m_transform.Resize(Size2D{10,10});
    m_transform.Upscale(2);
    const auto center = m_transform.GetCenter();
    ASSERT_EQ(center.x, 10);
    ASSERT_EQ(center.y, 10);
}

TRANSFORM_TEST(CheckMove) {
    m_transform.SetPosition(Pos2D{10,10});
    m_transform.Move(Pos2D{13, 57});
    const auto pos = m_transform.GetPosition();
    ASSERT_EQ(pos.x, 23);
    ASSERT_EQ(pos.y, 67);
}

TRANSFORM_TEST(CheckRotate) {
    m_transform.SetAngle(10);
    m_transform.Rotate(5);
    ASSERT_EQ(m_transform.GetAngle(), 15.0);
}

TRANSFORM_TEST(CheckNegativeRotate) {
    m_transform.SetAngle(10);
    m_transform.Rotate(-5);
    ASSERT_EQ(m_transform.GetAngle(), 5.0);
}

TRANSFORM_TEST(CheckRotateOverflow) {
    m_transform.SetAngle(350);
    m_transform.Rotate(20);
    ASSERT_EQ(m_transform.GetAngle(), 10.0);
}

TRANSFORM_TEST(CheckNegativeRotateOverflow) {
    m_transform.SetAngle(-350);
    m_transform.Rotate(-20);
    ASSERT_EQ(m_transform.GetAngle(), -10.0);
}

TRANSFORM_TEST(CheckVerticalFlip) {
    m_transform.FlipVertically();
    ASSERT_EQ(m_transform.IsFlippedVertically(), true);
}

TRANSFORM_TEST(CheckHorizontalFlip) {
    m_transform.FlipHorizontally();
    ASSERT_EQ(m_transform.IsFlippedHorizontally(), true);
}

TRANSFORM_TEST(CheckVerticalFlipToggle) {
    m_transform.FlipVertically();
    m_transform.FlipVertically();
    ASSERT_EQ(m_transform.IsFlippedVertically(), false) << "Should work as toggle";
}

TRANSFORM_TEST(CheckHorizontalFlipToggle) {
    m_transform.FlipHorizontally();
    m_transform.FlipHorizontally();
    ASSERT_EQ(m_transform.IsFlippedHorizontally(), false) << "Should work as toggle";
}

TRANSFORM_TEST(CheckCombinedFlip) {
    m_transform.FlipVertically();
    m_transform.FlipHorizontally();
    ASSERT_EQ(m_transform.IsFlippedVertically(), true);
    ASSERT_EQ(m_transform.IsFlippedHorizontally(), true);
}

TRANSFORM_TEST(CheckCombinedFlipCancelVertical) {
    m_transform.FlipVertically();
    m_transform.FlipHorizontally();
    m_transform.FlipVertically(); // cancel vertical flip
    ASSERT_EQ(m_transform.IsFlippedVertically(), false) << "Should work as toggle and don't affect other flip";
    ASSERT_EQ(m_transform.IsFlippedHorizontally(), true);
}

TRANSFORM_TEST(CheckCombinedFlipCancelHorizontal) {
    m_transform.FlipVertically();
    m_transform.FlipHorizontally();
    m_transform.FlipHorizontally(); // cancel horizontal flip
    ASSERT_EQ(m_transform.IsFlippedVertically(), true);
    ASSERT_EQ(m_transform.IsFlippedHorizontally(), false) << "Should work as toggle and don't affect other flip";
}


