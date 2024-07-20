#include <Logger.h>

#include <gtest/gtest.h>

using namespace GameEngine;

TEST(ArchitectureTest, ShouldBe64bit) 
{
    ASSERT_EQ(sizeof(size_t), 8) << "The architecture should be 64bit!";
}

TEST(Logger, ShouldLogClassMethod)
{
    // Just to demonstrate log format
    struct MyClassWithLog : private Logable
    {
        MyClassWithLog() 
            : Logable("MyClass") 
        {}

        void Method()
        {
            LOG_TRACE("Hello from method");
        }
    };

    MyClassWithLog cls;
    cls.Method();
}
