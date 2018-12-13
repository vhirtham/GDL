#[==[
Applies the default build setup to the target
#]==]

function(TargetDefaultBuildSetup target)

    target_include_directories(${target}
        PUBLIC
            ${GDL_ROOT_DIR})

    target_compile_features(${target}
        PUBLIC
            ${GDL_COMPILE_FEATURES})

    target_compile_definitions(${target}
        PUBLIC
            ${GDL_COMPILE_DEFINITIONS})

endfunction()
