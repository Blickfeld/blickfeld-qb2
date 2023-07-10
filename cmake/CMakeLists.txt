# CMake Package
include(CMakePackageConfigHelpers)

# Generate package
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
export(EXPORT ${PROJECT_NAME}
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}.cmake"
)
configure_file(${PROJECT_NAME}-config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-config.cmake"
    @ONLY
)

# Install
set(ConfigPackageLocation lib/cmake/${PROJECT_NAME})
install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}.cmake
    DESTINATION ${ConfigPackageLocation}
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION ${ConfigPackageLocation}
    COMPONENT Devel
)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${ConfigPackageLocation}
    COMPONENT Devel
    FILES_MATCHING 
    PATTERN "third-party/*.cmake"
)
