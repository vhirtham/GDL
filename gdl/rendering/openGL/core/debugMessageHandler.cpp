#include "gdl/rendering/openGL/core/debugMessageHandler.h"


#include "gdl/base/fundamentalTypes.h"

#include <iostream>


namespace GDL::OpenGL
{

DebugMessageHandler::DebugMessageHandler()
    : mTypeOutputMethod{{GL_DEBUG_TYPE_ERROR, {OutputMethod::EXCEPTION, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_PORTABILITY, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_PERFORMANCE, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_MARKER, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_OTHER, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}}}
{
}



void DebugMessageHandler::Initialize()
{
    if (mInitialized)
        return;

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    glDebugMessageCallback(DebugCallback, this);

    mInitialized = true;
}



bool DebugMessageHandler::IsInitialized() const
{
    return mInitialized;
}



OutputMethod DebugMessageHandler::GetOutputMethod(GLenum type) const
{
    return GetMessageTypeSetup(type).outputMethod;
}



void DebugMessageHandler::SetOutputMethod(GLenum type, OutputMethod outputMethod)
{
    GetMessageTypeSetup(type).outputMethod = outputMethod;
}



GLenum DebugMessageHandler::GetMinimalSeverityLevel(GLenum type) const
{
    return GetMessageTypeSetup(type).severityLevel;
}



void DebugMessageHandler::SetMinimalSeverityLevel(GLenum type, GLenum severity)
{
    GetMessageTypeSetup(type).severityLevel = severity;
}



void DebugMessageHandler::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                        [[maybe_unused]] GLsizei length, const GLchar* message, const void* userParam)
{
    DEV_EXCEPTION(userParam == nullptr,
                  "You passed a nullptr as userParam. It must contain the address of a DebugMessageHandlers.");
    const DebugMessageHandler& debugMessageHandler = *(reinterpret_cast<const DebugMessageHandler*>(userParam));

    const U32 severityLevel = GetSeverityInteger(severity);
    const U32 minimalSeverityLevel = GetSeverityInteger(debugMessageHandler.GetMinimalSeverityLevel(type));
    const OutputMethod outputMethod = debugMessageHandler.GetOutputMethod(type);

    if (outputMethod == OutputMethod::NONE || severityLevel < minimalSeverityLevel)
        return;

    String outputMessage = GenerateMessage(source, type, id, severity, message);

    ForwardOutputMessage(outputMethod, outputMessage);
}



DebugMessageHandler::MessageTypeSetup& DebugMessageHandler::GetMessageTypeSetup(GLenum type)
{
    auto iter = mTypeOutputMethod.find(type);
    EXCEPTION(iter == mTypeOutputMethod.end(), "Invalid type");
    return iter->second;
}



const DebugMessageHandler::MessageTypeSetup& DebugMessageHandler::GetMessageTypeSetup(GLenum type) const
{
    auto iter = mTypeOutputMethod.find(type);
    EXCEPTION(iter == mTypeOutputMethod.end(), "Invalid type");
    return iter->second;
}



U32 DebugMessageHandler::GetSeverityInteger(GLenum severity)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        return 3;
    case GL_DEBUG_SEVERITY_MEDIUM:
        return 2;
    case GL_DEBUG_SEVERITY_LOW:
        return 1;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return 0;
    default:
        return 3;
    }
}



const char* DebugMessageHandler::GetSeverityString(GLenum severity)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        return "High";
    case GL_DEBUG_SEVERITY_MEDIUM:
        return "Medium";
    case GL_DEBUG_SEVERITY_LOW:
        return "Low";
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return "Notification";
    default:
        return ("Unknown");
    }
}



const char* DebugMessageHandler::GetSourceString(GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return "Window system";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return "Shader compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        return "Third party";
    case GL_DEBUG_SOURCE_APPLICATION:
        return "Application";
    case GL_DEBUG_SOURCE_OTHER:
        return "Other";
    default:
        return "Unknown";
    }
}



const char* DebugMessageHandler::GetTypeString(GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return "Deprecated behavior";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return "Undefined behavior";
    case GL_DEBUG_TYPE_PORTABILITY:
        return "Portability issue";
    case GL_DEBUG_TYPE_PERFORMANCE:
        return "Performance issue";
    case GL_DEBUG_TYPE_MARKER:
        return "Stream annotation";
    case GL_DEBUG_TYPE_OTHER:
        return "Other";
    default:
        return "Unknown";
    }
}



String DebugMessageHandler::GenerateMessage(GLenum source, GLenum type, GLuint id, GLenum severity,
                                            const GLchar* message)
{
    // clang-format off
    return MakeString("OpenGL debug message",
                    "\n--------------------",
                    "\nSource   : ", GetSourceString(source),
                    "\nType     : ", GetTypeString(type),
                    "\nID       : ", ToString(id),
                    "\nSeverity : ", GetSeverityString(severity),
                    "\nMessage  : ", message, "\n");
    // clang-format on
}



void DebugMessageHandler::ForwardOutputMessage(OutputMethod outputMethod, const String& outputMessage)
{
    switch (outputMethod)
    {
    case OutputMethod::COUT:
        std::cout << outputMessage << std::endl;
        break;
    default:
        THROW(MakeString("\nCritical OpenGL debug message occurred:\n", outputMessage).c_str());
    }
}

} // namespace GDL::OpenGL
