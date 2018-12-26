#include <boost/test/unit_test.hpp>

#include "gdl/base/exception.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/rendering/openGL/core/bufferObject.h"
#include "gdl/rendering/openGL/core/vertexArrayObject.h"

#include "test/tools/ExceptionChecks.h"
#include "test/tools/GetRenderWindowGLUT.h"

using namespace GDL;
using namespace GDL::OpenGL;



BOOST_AUTO_TEST_CASE(Enable_Attributes)
{
    GetRenderWindowGLUT();
    ContextManagerGLUT::Instance().GetDebugMessageHandler().SetSeverityLevel(GL_DEBUG_SEVERITY_LOW);

    VertexArrayObject vao;

    BufferObject vbo1(120, GL_STATIC_DRAW);
    BufferObject vbo2(40, GL_STATIC_DRAW);

    BOOST_CHECK(vao.QueryNumEnabledAttributes() == 0);
    BOOST_CHECK(vao.QueryIsAttributeEnabled(0) == false);
    BOOST_CHECK(vao.QueryIsAttributeEnabled(3) == false);
    vao.EnableAttribute(0, 0, GL_FLOAT, 1);
    vao.EnableAttribute(3, 1, GL_FLOAT, 1, 0, GL_FALSE);
    BOOST_CHECK(vao.QueryAreAllVertexAttribsBoundToBuffers() == false);
    BOOST_CHECK(vao.QueryNumEnabledAttributes() == 2);
    BOOST_CHECK(vao.QueryIsAttributeEnabled(0) == true);
    BOOST_CHECK(vao.QueryIsAttributeEnabled(3) == true);


    BOOST_CHECK(vao.QueryAttributeBuffer(0) == 0);
    BOOST_CHECK(vao.QueryAttributeBuffer(3) == 0);
    vao.SetBufferBinding(0, vbo1, 8, 0);
    BOOST_CHECK(vao.QueryAreAllVertexAttribsBoundToBuffers() == false);
    BOOST_CHECK(vao.QueryAttributeBuffer(0) == vbo1.GetHandle());

    BOOST_CHECK(vao.QueryAttributeBuffer(3) == 0);
    vao.SetBufferBinding(1, vbo1, 4, 80);
    BOOST_CHECK(vao.QueryAreAllVertexAttribsBoundToBuffers() == true);
    BOOST_CHECK(vao.QueryAttributeBuffer(3) == vbo1.GetHandle());

    // Enable with binding point that is already connected to a buffer
    BOOST_CHECK(vao.QueryIsAttributeEnabled(2) == false);
    vao.EnableAttribute(2, 0, GL_FLOAT, 1, 4);
    BOOST_CHECK(vao.QueryAreAllVertexAttribsBoundToBuffers() == true);
    BOOST_CHECK(vao.QueryIsAttributeEnabled(2) == true);
    BOOST_CHECK(vao.QueryAttributeBuffer(2) == vbo1.GetHandle());
    BOOST_CHECK(vao.QueryNumEnabledAttributes() == 3);

    BOOST_CHECK(vao.QueryIsAttributeEnabled(7) == false);
    vao.EnableAttribute(7, 5, vbo2, GL_FLOAT, 1, 1, 0);
    BOOST_CHECK(vao.QueryAreAllVertexAttribsBoundToBuffers() == true);
    BOOST_CHECK(vao.QueryIsAttributeEnabled(7) == true);
    BOOST_CHECK(vao.QueryAttributeBuffer(7) == vbo2.GetHandle());
    BOOST_CHECK(vao.QueryNumEnabledAttributes() == 4);

    vao.Bind();
}


BOOST_AUTO_TEST_CASE(Exceptions)
{
    VertexArrayObject vao;

    BufferObject vbo1(40, GL_STATIC_DRAW);
    BufferObject vbo2(40, GL_STATIC_DRAW);

    vao.EnableAttribute(0, 0, GL_FLOAT, 1);
    vao.EnableAttribute(3, 1, GL_FLOAT, 1, 0, GL_FALSE);

    // Attributes without buffer
    GDL_CHECK_THROW_DEBUG(vao.Bind(), Exception);

    // Attribute already enabled
    GDL_CHECK_THROW_DEV_DISABLE(vao.EnableAttribute(0, 0, GL_FLOAT, 1, 0, GL_FALSE), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(vao.EnableAttribute(0, 0, vbo1, GL_FLOAT, 1, 1), Exception);

    // Invalid stride
    GDL_CHECK_THROW_DEV_DISABLE(vao.SetBufferBinding(0, vbo1, 0), Exception);

    // Invalid offset
    GDL_CHECK_THROW_DEV_DISABLE(vao.SetBufferBinding(0, vbo1, 1, -1), Exception);
}
