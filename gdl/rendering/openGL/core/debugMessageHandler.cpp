#include "gdl/rendering/openGL/core/debugMessageHandler.h"


#include "gdl/base/fundamentalTypes.h"

#include <iostream>


namespace GDL::OpenGL
{

DebugMessageHandler::DebugMessageHandler()
    : mMessageTypeSetup{{GL_DEBUG_TYPE_ERROR, {OutputMethod::EXCEPTION, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_PORTABILITY, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_PERFORMANCE, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_MARKER, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}},
                        {GL_DEBUG_TYPE_OTHER, {OutputMethod::COUT, GL_DEBUG_SEVERITY_NOTIFICATION}}}
{
}



OutputMethod DebugMessageHandler::GetMessageTypeOutputMethod(GLenum type) const
{
    return GetMessageTypeSetup(type).outputMethod;
}



GLenum DebugMessageHandler::GetMessageTypeSeverityLevel(GLenum type) const
{
    return GetMessageTypeSetup(type).severityLevel;
}



void DebugMessageHandler::Initialize()
{
    EXCEPTION(!IsContextDebug(),
              "Can not initialize the debug message handler. The OpenGL context is not a debug context.");

    if (mInitialized)
        return;

    glDebugMessageCallback(DebugCallback, this);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);


    mInitialized = true;
}



bool DebugMessageHandler::IsInitialized() const
{
    return mInitialized;
}



bool DebugMessageHandler::IsContextDebug()
{
    GLint contextFlags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);
    return (contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT);
}



void DebugMessageHandler::SetMessageTypeOutputMethod(GLenum type, OutputMethod outputMethod)
{
    GetMessageTypeSetup(type).outputMethod = outputMethod;
}



void DebugMessageHandler::SetMessageTypeSeverityLevel(GLenum type, GLenum severity)
{
    DEV_EXCEPTION(!IsSeverityValid(severity), "The value of the severity enum does not belong to a severity level");
    GetMessageTypeSetup(type).severityLevel = severity;
}



void DebugMessageHandler::SetOutputMethod(OutputMethod outputMethod)
{
    for (auto& keyValuePair : mMessageTypeSetup)
        keyValuePair.second.outputMethod = outputMethod;
}



void DebugMessageHandler::SetSeverityLevel(GLenum severity)
{
    DEV_EXCEPTION(!IsSeverityValid(severity), "The value of the severity enum does not belong to a severity level");
    for (auto& keyValuePair : mMessageTypeSetup)
        keyValuePair.second.severityLevel = severity;
}



void DebugMessageHandler::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                        [[maybe_unused]] GLsizei length, const GLchar* message, const void* userParam)
{
    DEV_EXCEPTION(userParam == nullptr,
                  "You passed a nullptr as userParam. It must contain the address of a DebugMessageHandlers.");
    const DebugMessageHandler& debugMessageHandler = *(reinterpret_cast<const DebugMessageHandler*>(userParam));

    const U32 severityLevel = GetSeverityInteger(severity);

    MessageTypeSetup messageTypeSetup = debugMessageHandler.GetMessageTypeSetup(type);
    const U32 minimalSeverityLevel = GetSeverityInteger(messageTypeSetup.severityLevel);

    if (messageTypeSetup.outputMethod == OutputMethod::NONE || severityLevel < minimalSeverityLevel)
        return;

    String outputMessage = GenerateMessage(source, type, id, severity, message);

    ForwardOutputMessage(messageTypeSetup.outputMethod, outputMessage);
}



void DebugMessageHandler::ForwardOutputMessage(OutputMethod outputMethod, const String& outputMessage)
{
    switch (outputMethod)
    {
    case OutputMethod::COUT:
        std::cout << outputMessage << std::endl;
        std::cout.flush();
        break;
    default:
        THROW(MakeString("\nCritical OpenGL debug message occurred:\n", outputMessage).c_str());
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



DebugMessageHandler::MessageTypeSetup& DebugMessageHandler::GetMessageTypeSetup(GLenum type)
{
    auto iter = mMessageTypeSetup.find(type);
    EXCEPTION(iter == mMessageTypeSetup.end(), "Invalid type enum");
    return iter->second;
}



const DebugMessageHandler::MessageTypeSetup& DebugMessageHandler::GetMessageTypeSetup(GLenum type) const
{
    auto iter = mMessageTypeSetup.find(type);
    EXCEPTION(iter == mMessageTypeSetup.end(), "Invalid type enum");
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



bool DebugMessageHandler::IsSeverityValid(GLenum severity)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
    case GL_DEBUG_SEVERITY_MEDIUM:
    case GL_DEBUG_SEVERITY_LOW:
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return true;
    default:
        return false;
    }
}



} // namespace GDL::OpenGL
