#include <boost/test/unit_test.hpp>

#include "gdl/rendering/openGL/core/contextGLUT.h"
#include "gdl/rendering/openGL/core/renderWindowGLUT.h"
#include "test/tools/ExceptionChecks.h"


using namespace GDL;
using namespace GDL::OpenGL;


BOOST_AUTO_TEST_CASE(TestContextGLUT)
{
    ContextGLUT& contextGLUT = ContextGLUT::Instance();
    BOOST_CHECK(contextGLUT.IsDebug() == false);
    contextGLUT.EnableDebug();
    BOOST_CHECK(contextGLUT.IsDebug() == true);

    BOOST_CHECK(contextGLUT.IsInitialized() == false);
    contextGLUT.Initialize();
    BOOST_CHECK(contextGLUT.IsInitialized() == true);

    DebugMessageHandler& debugMessageHandler = ContextGLUT::Instance().GetDebugMessageHandler();

    BOOST_CHECK(debugMessageHandler.IsInitialized() == false);
    RenderWindowGLUT renderWindow(contextGLUT);
    renderWindow.Initialize();
    BOOST_CHECK(debugMessageHandler.IsInitialized() == true);
}



BOOST_AUTO_TEST_CASE(TestDebugMessageHandler)
{
    DebugMessageHandler& debugMessageHandler = ContextGLUT::Instance().GetDebugMessageHandler();



    // Test Getter / Setter
    // --------------------

    GLenum messageType = GL_DEBUG_TYPE_OTHER;
    OutputMethod currentOutputMethod = debugMessageHandler.GetMessageTypeOutputMethod(messageType);
    OutputMethod newOutputMethod = OutputMethod::NONE;
    GLenum currentSeverity = debugMessageHandler.GetMessageTypeSeverityLevel(messageType);
    GLenum newSeverity = GL_DEBUG_SEVERITY_LOW;

    // Check first that the current values are not equal to the new values, otherwise the following tests are useless
    BOOST_CHECK(currentOutputMethod != newOutputMethod);
    BOOST_CHECK(currentSeverity != newSeverity);

    debugMessageHandler.SetMessageTypeOutputMethod(messageType, newOutputMethod);
    BOOST_CHECK(debugMessageHandler.GetMessageTypeOutputMethod(messageType) == newOutputMethod);
    debugMessageHandler.SetMessageTypeSeverityLevel(messageType, newSeverity);
    BOOST_CHECK(debugMessageHandler.GetMessageTypeSeverityLevel(messageType) == newSeverity);

    BOOST_CHECK_THROW(debugMessageHandler.GetMessageTypeOutputMethod(GL_NONE), Exception);
    BOOST_CHECK_THROW(debugMessageHandler.GetMessageTypeSeverityLevel(GL_NONE), Exception);
    BOOST_CHECK_THROW(debugMessageHandler.SetMessageTypeOutputMethod(GL_NONE, newOutputMethod), Exception);
    BOOST_CHECK_THROW(debugMessageHandler.SetMessageTypeSeverityLevel(GL_NONE, newSeverity), Exception);
    GDL_CHECK_THROW_DEV_DISABLE(debugMessageHandler.SetMessageTypeSeverityLevel(messageType, GL_NONE), Exception);



    std::array<GLenum, 8> validMessageTypes = {GL_DEBUG_TYPE_ERROR,
                                               GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
                                               GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
                                               GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
                                               GL_DEBUG_TYPE_PORTABILITY,
                                               GL_DEBUG_TYPE_PERFORMANCE,
                                               GL_DEBUG_TYPE_MARKER,
                                               GL_DEBUG_TYPE_OTHER};

    newOutputMethod = OutputMethod::LOGGER;
    newSeverity = GL_DEBUG_SEVERITY_MEDIUM;

    for (GLenum type : validMessageTypes)
    {
        BOOST_CHECK(debugMessageHandler.GetMessageTypeOutputMethod(type) != newOutputMethod);
        BOOST_CHECK(debugMessageHandler.GetMessageTypeSeverityLevel(type) != newSeverity);
    }

    debugMessageHandler.SetOutputMethod(newOutputMethod);
    debugMessageHandler.SetSeverityLevel(newSeverity);

    for (GLenum type : validMessageTypes)
    {
        BOOST_CHECK(debugMessageHandler.GetMessageTypeOutputMethod(type) == newOutputMethod);
        BOOST_CHECK(debugMessageHandler.GetMessageTypeSeverityLevel(type) == newSeverity);
    }

    GDL_CHECK_THROW_DEV_DISABLE(debugMessageHandler.SetSeverityLevel(GL_NONE), Exception);
}
