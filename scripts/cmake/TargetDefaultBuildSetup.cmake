#[==[
Applies the default build setup to the target
#]==]

function(TargetDefaultBuildSetup target)

    target_include_directories(${target}
        PUBLIC
            ${GDL_SOURCE_DIR})

    target_compile_features(${target}
        PUBLIC
            ${GDL_COMPILE_FEATURES})

endfunction()
