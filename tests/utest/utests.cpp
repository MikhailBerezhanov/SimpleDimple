#include <gtest/gtest.h>

TEST(Common, Arch_64bit_check) {
    ASSERT_EQ(sizeof(size_t), 8) << "The architecture should be 64bit!";
}