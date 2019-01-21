#pragma once

#include "gdl/base/container/map.h"
#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/string.h"

#include <GL/glew.h>

namespace GDL::OpenGL
{

enum class OutputMethod
{
    COUT,
    EXCEPTION,
    LOGGER,
    NONE
};

//! @brief Processes the log messages that are generated by the OpenGL 4.3 debug context
class DebugMessageHandler
{

    struct MessageTypeSetup
    {
        OutputMethod outputMethod;
        GLenum severityLevel;
    };

    Map<GLenum, MessageTypeSetup> mMessageTypeSetup;
    bool mInitialized = false;



public:
    DebugMessageHandler();
    DebugMessageHandler(const DebugMessageHandler&) = delete;
    DebugMessageHandler(DebugMessageHandler&&) = delete;
    DebugMessageHandler& operator=(const DebugMessageHandler&) = delete;
    DebugMessageHandler& operator=(DebugMessageHandler&&) = delete;
    ~DebugMessageHandler() = default;

    //! @brief Gets the output method of the specified message type
    //! @param type: Type enum
    //! @return Output method enum
    OutputMethod GetMessageTypeOutputMethod(GLenum type) const;

    //! @brief Gets the minimal severity that a message of the specified type must have to be processed.
    //! @param type: Type enum
    //! @return Minimal severity that a message of the specified type must have to be processed.
    GLenum GetMessageTypeSeverityLevel(GLenum type) const;

    //! @brief Sets the OpenGL debug callback function. This is only done during the first function call. Afterwards it
    //! does nothing to avoid accidentally overwriting user defined debug callbacks.
    void Initialize();

    //! @brief Returns if the class is already initialized
    //! @return True / False
    bool IsInitialized() const;


    //! Checks if the OpenGL context is a debug context
    static bool IsContextDebug();

    //! @brief Sets the output method of the specified message type
    //! @param type: Type enum
    //! @param outputMethod: Output method enum
    void SetMessageTypeOutputMethod(GLenum type, OutputMethod outputMethod);

    //! @brief Sets the minimal severity level for the message type. If the severity of a message is lower it will be
    //! ignored.
    //! @param type: Type enum
    //! @param severity: Severity enum
    void SetMessageTypeSeverityLevel(GLenum type, GLenum severity);

    //! @brief Sets the output method for all message types
    //! @param outputMethod: Output method enum
    void SetOutputMethod(OutputMethod outputMethod);

    //! @brief Sets the minimal severity level for all message types. If the severity of a message is lower it will be
    //! ignored.
    //! @param severity: Severity enum
    void SetSeverityLevel(GLenum severity);

private:
    //! @brief Debug callback function that is registered to the debug context if a instance of this class is
    //! initialized
    //! @param source: Source enum
    //! @param type: Type enum
    //! @param id: Message id
    //! @param severity: Severity enum
    //! @param length: Length of the message
    //! @param message: The error message
    //! @param userParam: Address of an instance of this class
    //! @remark The interface of this class is prescribed by the OpenGL API
    static void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar* message, const void* userParam);

    //! @brief Forwards the output message to the specified output system
    //! @param outputMethod: Output method enum
    //! @param outputMessage: Output message
    static void ForwardOutputMessage(OutputMethod outputMethod, const String& outputMessage);

    //! @brief Generates a debug message from the debug callbacks input parameters
    //! @param source: Source enum
    //! @param type: Type enum
    //! @param id: Message id
    //! @param severity: Severity enum
    //! @param message: The error message
    //! @return Debug message
    static String GenerateMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const GLchar* message);

    //! @brief Gets the setup struct for the specified message type
    //! @param type: Type enum
    //! @return Setup struct for the specified message type
    MessageTypeSetup& GetMessageTypeSetup(GLenum type);

    //! @brief Gets the setup struct for the specified message type
    //! @param type: Type enum
    //! @return Setup struct for the specified message type
    const MessageTypeSetup& GetMessageTypeSetup(GLenum type) const;

    //! @brief Returns a number that corresponds to the importance of the message. The number increases with the
    //! messages importance.
    //! @param severity: Severity enum
    //! @return Number that corresponds to the importance of the message
    static U32 GetSeverityInteger(GLenum severity);

    //! @brief Casts the severity enum to a string
    //! @param severity: Severity enum
    //! @return Severity enum as string
    static const char* GetSeverityString(GLenum severity);

    //! @brief Casts the source enum to a string
    //! @param source: Source enum
    //! @return Source enum as string
    static const char* GetSourceString(GLenum source);

    //! @brief Casts the type enum to a string
    //! @param type: Type enum
    //! @return Type enum as string
    static const char* GetTypeString(GLenum type);

    //! @brief Checks if the enum value represents a severity level
    //! @param severity: Severity enum
    //! @return True / False
    static bool IsSeverityValid(GLenum severity);
};


} // namespace GDL::OpenGL
