function(CreateSingleSourceObjectLib Source Object)
    ### Target name
    singleSourceTargetName(${Source} libName)
    ### Create object lib
    add_library(${libName} OBJECT ${Source})
    ### Add src directory
    target_include_directories(${libName} PUBLIC ${CMAKE_SOURCE_DIR})
    ### Add compile features
    target_compile_features(${libName} PUBLIC ${GDL_COMPILE_FEATURES})
    ### Make target global
    set(${Object} "$<TARGET_OBJECTS:${libName}>" PARENT_SCOPE)
endfunction()
