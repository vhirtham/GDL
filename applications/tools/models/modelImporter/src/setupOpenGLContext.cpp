#include "applications/tools/models/modelImporter/src/setupOpenGLContext.h"

#include <QOpenGLContext>

void SetupOpenGLContext(QOpenGLContext* context)
{
    QSurfaceFormat format;

    format.setMajorVersion(4);
    format.setMinorVersion(6);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setOption(QSurfaceFormat::DebugContext);
    context->setFormat(format);
    context->create();
}
