addGDLOpenGLApp(TexturedRectangle
    texturedRectangle.cpp)

target_compile_definitions(TexturedRectangle
    PRIVATE
    TEXTURE_DIRECTORY="${GDL_ROOT_DIR}/applications/examples/data/textures")
