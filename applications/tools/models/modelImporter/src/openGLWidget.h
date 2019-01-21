#pragma once


#include "gdl/rendering/openGL/core/debugMessageHandler.h"
//#include "QOpenGLContext"
//#include <GL/glew.h>
#include <QOpenGLWidget>



class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

    GDL::OpenGL::DebugMessageHandler mDebugMessageHandler;

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    virtual ~OpenGLWidget() override;

private:
    virtual void resizeGL(int w, int h) override;
    virtual void initializeGL() override;
    virtual void paintGL() override;
};
