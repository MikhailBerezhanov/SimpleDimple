# These vars need to be provided for tests:
# GTEST_INCLUDE_DIR
# GTEST_SOURCES
# GTEST_LINK_LIBS

# set utest exe name
set(TEST_EXE "utest")

include(FetchContent)

# Fetch Google Test from GitHub
FetchContent_Declare(
    googletest
    URL "https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip"
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE # suppress warning
)

FetchContent_MakeAvailable(googletest)

# Add test sources
add_executable(${TEST_EXE} ${GTEST_SOURCES} test_main.cpp)

# Include .h files location
target_include_directories(${TEST_EXE} PRIVATE ${GTEST_INCLUDE_DIR})

# Link libs
target_link_libraries(${TEST_EXE} ${GTEST_LINK_LIBS})

# Link gtest libs to test exe
target_link_libraries(
    ${TEST_EXE}
    GTest::gtest_main
    GTest::gmock_main
)
