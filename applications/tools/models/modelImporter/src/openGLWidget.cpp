
#include "openGLWidget.h"

#include "gdl/base/string.h"
#include "gdl/rendering/openGL/core/glewController.h"
#include "applications/tools/models/modelImporter/src/model.h"


using namespace MI;
using namespace GDL;

Model& GetModel()
{
    static Model model(MakeString(MODEL_DIRECTORY, "/drawer/drawer.obj").c_str());
    return model;
}


OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)

{
}



OpenGLWidget::~OpenGLWidget()
{
}



void OpenGLWidget::resizeGL([[maybe_unused]] int w, [[maybe_unused]] int h)
{
}



void OpenGLWidget::initializeGL()
{
    GDL::OpenGL::GLEWController::Instance().Initialize();
}



void OpenGLWidget::paintGL()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    GetModel().Draw();
    // glDrawArrays(GL_TRIANGLES, 0, 3);
}
