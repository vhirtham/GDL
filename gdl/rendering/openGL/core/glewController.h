#pragma once

namespace GDL::OpenGL
{


//! @brief Small singleton wrapper for GLEW
class GLEWController
{
    bool mInitialized = false;

    GLEWController() = default;

public:
    GLEWController(const GLEWController&) = delete;
    GLEWController(GLEWController&&) = delete;
    GLEWController& operator=(const GLEWController&) = delete;
    GLEWController& operator=(GLEWController&&) = delete;
    ~GLEWController() = default;

    //! @brief Gets the singleton instance of the GLEW controller
    //! @return Instance of the GLEW controller
    static GLEWController& Instance();

    //! @brief Initializes the class
    void Initialize();

    //! @brief Returns if the class is already initialized
    //! @return True / False
    bool IsInitialized() const;
};


} // namespace GDL::OpenGL
