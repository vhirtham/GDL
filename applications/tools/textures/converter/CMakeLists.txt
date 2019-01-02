add_executable(TextureConverter
    textureConverter.cpp
    ${GDL_ROOT_DIR}/gdl/rendering/textures/textureData.cpp
    ${GDL_ROOT_DIR}/gdl/rendering/textures/textureFile.cpp
    )

TargetDefaultBuildSetup(TextureConverter)

target_link_libraries(TextureConverter
    GDL::Resources
    -lstdc++fs)

#target_compile_definitions(TextureConverter
#    PRIVATE
#    DATA_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/data")
