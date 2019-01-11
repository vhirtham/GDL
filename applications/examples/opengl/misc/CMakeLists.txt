addGDLOpenGLApp(MovingCamera
    movingCamera.cpp)

target_compile_definitions(MovingCamera
    PRIVATE
    TEXTURE_DIRECTORY="${GDL_ROOT_DIR}/applications/examples/data/textures")
