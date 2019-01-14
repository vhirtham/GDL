addGDLOpenGLApp(DiffusePerVertexShading
    diffusePerVertexShading.cpp)

addGDLOpenGLApp(PhongModel
    phongModel.cpp)

target_compile_definitions(PhongModel
    PRIVATE
    TEXTURE_DIRECTORY="${GDL_ROOT_DIR}/applications/examples/data/textures")
