#[==[
    Find GLFW
#]==]

function(findGLFW)
    # On Windows, have a look into this thread:
    # http://discourse.glfw.org/t/glfw-with-cmake-on-windows-glfw-include-dirs-or-glfw-libraries-not-set/890

    #find_package(glfw3 3.2 REQUIRED)

    #message("GLFW found: ${GLFW_LIBRARIES}")


    if(UNIX)
        find_package(PkgConfig)

        if(PKG_CONFIG_FOUND)
            pkg_search_module(GLFW glfw3)
            if(${GLFW_FOUND})
                # Find dependencies of GLFW
                execute_process(COMMAND "pkg-config" "--static" "--libs" "glfw3" OUTPUT_VARIABLE GLFW_DEPENDENCIES)
                string(REGEX REPLACE "\n$" "" GLFW_DEPENDENCIES ${GLFW_DEPENDENCIES}) #remove trailing whitespace
                set(GLFW_DEPENDENCIES ${GLFW_DEPENDENCIES} CACHE INTERNAL "")

                include_directories(${GLFW_INCLUDE_DIRS})
                #add_library(GLFW::GLFW ALIAS ${GLFW_LIBRARIES}) # Not possible yet
            endif()
        endif()

    elseif()
        message(WARNING "The cmake find script for GLFW is not implemented except for Linux. GLFW can't be used.")
    endif(UNIX)
endfunction()
