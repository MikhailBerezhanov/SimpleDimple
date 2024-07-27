#include <GameObject.h>
#include <RenderContext.h>
#include <memory>
#include <gtest/gtest.h>

#define FIXTURE GameObjectTest
#define GAME_OBJ_TEST(NAME) TEST_F(FIXTURE, NAME)

using namespace GameEngine;
using ExceptionType = std::exception;

// derived from RenderContext
class RenderContextTest : public RenderContext {
public:
    RenderContextTest() : RenderContext() {}
};

// test fixture
class FIXTURE : public testing::Test {
protected:
    GameObject m_gameObject;
    explicit FIXTURE()
        : m_gameObject("test") {}
};

GAME_OBJ_TEST(TransformNoExcept) {
    ASSERT_NO_THROW(m_gameObject.AddComponent(GameObjectComponentType::TRANSFORM));
}

GAME_OBJ_TEST(TransformWithSizeNoExcept) {
    ASSERT_NO_THROW(m_gameObject.AddComponent(GameObjectComponentType::TRANSFORM, Size2D{1,1}));
}

GAME_OBJ_TEST(NoRendererWithoutTransform) {
    ASSERT_THROW(m_gameObject.AddComponent(GameObjectComponentType::RENDERER), ExceptionType)
    << "It shouldn't be possible to add renderer component without transform component";
}

GAME_OBJ_TEST(NoTextureWithoutRenderer) {
    ASSERT_THROW(m_gameObject.AddComponent(GameObjectComponentType::TEXTURE), ExceptionType)
    << "It shouldn't be possible to add texture component without renderer component";
}

GAME_OBJ_TEST(NoDuplicates) {
    m_gameObject.AddComponent(GameObjectComponentType::TRANSFORM);
    ASSERT_THROW(m_gameObject.AddComponent(GameObjectComponentType::TRANSFORM), ExceptionType)
    << "It shouldn't be possible to add already existing component";
}

GAME_OBJ_TEST(NoExceptRendererWithTransform) {
    auto context = std::make_unique<RenderContextTest>();
    auto context_cast = dynamic_cast<RenderContext*>(context.get()); // dummy context for renderer
    m_gameObject.AddComponent(GameObjectComponentType::TRANSFORM);
    ASSERT_NO_THROW(m_gameObject.AddComponent(GameObjectComponentType::RENDERER, *context_cast));
}


