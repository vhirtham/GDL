
#include "openGLWidget.h"
#include "gdl/rendering/openGL/core/glewController.h"


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
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
