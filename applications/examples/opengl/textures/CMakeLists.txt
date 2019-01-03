addGDLOpenGLApp(TexturedRectangle
    texturedRectangle.cpp)

target_compile_definitions(TexturedRectangle
    PRIVATE
    TEXTURE_DIRECTORY="${GDL_ROOT_DIR}/applications/examples/data/textures")


addGDLOpenGLApp(TextureWrapping
    textureWrapping.cpp)

target_compile_definitions(TextureWrapping
    PRIVATE
    TEXTURE_DIRECTORY="${GDL_ROOT_DIR}/applications/examples/data/textures")


addGDLOpenGLApp(TextureFiltering
    textureFiltering.cpp)

target_compile_definitions(TextureFiltering
    PRIVATE
    TEXTURE_DIRECTORY="${GDL_ROOT_DIR}/applications/examples/data/textures")
