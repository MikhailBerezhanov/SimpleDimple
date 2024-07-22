#include <GameObject.h>
#include <gtest/gtest.h>

#define FIXTURE GameObjectTest
#define GAME_OBJ_TEST(NAME) TEST_F(FIXTURE, NAME)

using namespace GameEngine;
using ExceptionType = std::exception;

// test fixture
class FIXTURE : public testing::Test {
protected:
    GameObject m_gameObject;
    explicit FIXTURE()
        : m_gameObject("test") {}
};

GAME_OBJ_TEST(TransformNoError) {
    ASSERT_NO_THROW(m_gameObject.AddComponent(GameObjectComponentType::TRANSFORM))
    << "It should be possible to add transform component to empty object";
}

GAME_OBJ_TEST(NoRendererWithoutTransform) {
    ASSERT_THROW(m_gameObject.AddComponent(GameObjectComponentType::RENDERER), ExceptionType)
    << "It shouldn't be possible to add renderer component without transform component";
}

GAME_OBJ_TEST(NoTextureWithoutRenderer) {
    ASSERT_THROW(m_gameObject.AddComponent(GameObjectComponentType::TEXTURE), ExceptionType)
    << "It shouldn't be possible to add renderer component without transform component";
}

GAME_OBJ_TEST(NoDuplicates) {
    m_gameObject.AddComponent(GameObjectComponentType::TRANSFORM);
    ASSERT_THROW(m_gameObject.AddComponent(GameObjectComponentType::TRANSFORM), ExceptionType)
    << "It shouldn't be possible to add already existing component";
}


