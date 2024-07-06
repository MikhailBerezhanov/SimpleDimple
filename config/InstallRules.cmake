# Installation - common
install(TARGETS ${EXEC_NAME} DESTINATION ${INSTALL_DIR})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # Installation - windows-specific
    install(DIRECTORY ${SDL_BIN_DIR}/
        DESTINATION ${INSTALL_DIR}
        FILES_MATCHING PATTERN "*.dll")
else()
    # Installation - linux-specific
    install(DIRECTORY ${SDL_LIB_DIR}/
        DESTINATION ${INSTALL_DIR}
        FILES_MATCHING PATTERN "*.so" PATTERN "*.so*"
        PATTERN "cmake" EXCLUDE
        PATTERN "pkgconfig" EXCLUDE)
endif()