#[==[
Creates an executable and adds the passed sources, libs and definitions
#]==]

function(AddExecutable target sources libs definitions)

    add_executable(${target}
            ${sources})

    target_compile_definitions(${target}
            ${definitions})

    target_link_libraries(${target}
        ${libs})

endfunction()
