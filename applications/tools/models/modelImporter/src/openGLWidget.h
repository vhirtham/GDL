#pragma once


#include <GL/glew.h>

#include <QOpenGLWidget>



class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget* parent = nullptr);
    virtual ~OpenGLWidget() override;

private:

    virtual void resizeGL(int w, int h) override;
    virtual void initializeGL() override;
    virtual void paintGL() override;
};
