# These vars need to be provided for tests:
# GTEST_INCLUDE_DIR
# GTEST_SOURCES

# set utest exe name
set(TEST_EXE "utest")

include(FetchContent)

# Fetch Google Test from GitHub
FetchContent_Declare(
    googletest
    URL "https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip"
)

FetchContent_MakeAvailable(googletest)  

# Add test exec
add_executable(${TEST_EXE} ${GTEST_SOURCES} utests.cpp)

# Include .h files location
target_include_directories(${TEST_EXE} PRIVATE ${GTEST_INCLUDE_DIR})

# Link libs
target_link_libraries(${TEST_EXE} ${GTEST_LINK_LIBS})

# Link gtest libs to test exe
target_link_libraries(
    ${TEST_EXE}
    GTest::gtest_main
)

# discover test within test exe
include(GoogleTest)
gtest_discover_tests(${TEST_EXE})