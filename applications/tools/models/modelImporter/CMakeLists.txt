

qt5_wrap_cpp(MainWindow_H_MOC src/mainWindow.h)
qt5_wrap_cpp(OpenGLWidget_H_MOC src/openGLWidget.h)

add_executable(ModelImporter
    modelImporter.cpp
    src/mainWindow.cpp
    src/openGLWidget.cpp
    src/model.cpp
    src/mesh.cpp
    src/setupOpenGLContext.cpp
    ${MainWindow_H_MOC}
     ${OpenGLWidget_H_MOC}
    )

TargetDefaultBuildSetup(ModelImporter)

target_link_libraries (ModelImporter
    Qt5::Widgets
    GDL::OpenGLCore
    -lassimp
    )

target_include_directories(ModelImporter
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(ModelImporter
    PRIVATE
        MODEL_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/data")


