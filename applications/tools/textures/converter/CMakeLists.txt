add_executable(TextureConverter
    textureConverter.cpp)

TargetDefaultBuildSetup(TextureConverter)

target_compile_definitions(TextureConverter PRIVATE DATA_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/data")
