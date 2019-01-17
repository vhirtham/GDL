
set (MyProjectBin_src ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

qt5_wrap_cpp(MainWindow_H_MOC src/mainWindow.h)
qt5_wrap_ui (MainWindow_UI_MOC src/mainWindow.ui)

add_executable(ModelImporter
    modelImporter.cpp
    src/mainWindow.cpp
    ${MainWindow_H_MOC}
    ${MainWindow_UI_MOC})

TargetDefaultBuildSetup(ModelImporter)

target_link_libraries (ModelImporter
    Qt5::Widgets)

target_include_directories(ModelImporter
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})


