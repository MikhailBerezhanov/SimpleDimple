add_executable(multiple_windows multiple_windows/main.cpp)
target_include_directories(multiple_windows PRIVATE ${PROJECT_INCLUDE_DIR})
target_link_libraries(multiple_windows ${PROJECT_LINK_LIBS})