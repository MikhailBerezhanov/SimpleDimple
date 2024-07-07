# Location prefix (out/Linux)
set(INSTALL_LOCATION ${INSTALL_DIR}/${CMAKE_SYSTEM_NAME})
# Binaries/runtime libs location
set(INSTALL_LOCATION_BIN ${INSTALL_LOCATION}/bin)
# Assets location
set(INSTALL_LOCATION_ASSETS ${INSTALL_LOCATION}/assets)

# Installation - common
install(TARGETS ${EXEC_NAME} DESTINATION ${INSTALL_DIR_BIN})

# Installation - SDL
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # Installation - windows-specific
    install(DIRECTORY ${SDL_BIN_DIR}/
        DESTINATION ${INSTALL_LOCATION_BIN}
        FILES_MATCHING PATTERN "*.dll")
else()
    # Installation - linux-specific
    install(DIRECTORY ${SDL_LIB_DIR}/
        DESTINATION ${INSTALL_LOCATION_BIN}
        FILES_MATCHING PATTERN "*.so" PATTERN "*.so*"
        PATTERN "cmake" EXCLUDE
        PATTERN "pkgconfig" EXCLUDE)
endif()

# Installation - assets
install(DIRECTORY ${ASSETS_TO_INSTALL_DIR}/ 
    DESTINATION ${INSTALL_LOCATION_ASSETS}
)