# Installation - common
install(TARGETS ${EXEC_NAME} DESTINATION ${INSTALL_DIR})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # Installation - windows-specific
    install(FILES ${SDL_PREFIX}/bin/SDL2.dll 
        DESTINATION ${INSTALL_DIR})
else()
    # Installation - linux-specific
    install(DIRECTORY ${SDL_PREFIX}/lib/ 
        DESTINATION ${INSTALL_DIR}
        FILES_MATCHING PATTERN "*.so" PATTERN "*.so*"
        PATTERN "cmake" EXCLUDE
        PATTERN "pkgconfig" EXCLUDE)
endif()