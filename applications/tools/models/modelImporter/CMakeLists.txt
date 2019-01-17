

qt5_wrap_cpp(MainWindow_H_MOC src/mainWindow.h)
qt5_wrap_cpp(OpenGLWidget_H_MOC src/openGLWidget.h)

add_executable(ModelImporter
    modelImporter.cpp
    src/mainWindow.cpp
    src/openGLWidget.cpp
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


