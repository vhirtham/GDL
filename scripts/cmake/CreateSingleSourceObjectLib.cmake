function(CreateSingleSourceObjectLib Source Object)
    ### Target name
    singleSourceTargetName(${Source} libName)
    ### Create object lib
    add_library(${libName} OBJECT ${Source})
    ### Add src directory
    target_include_directories(${libName} PUBLIC ${CMAKE_SOURCE_DIR}/src)
    ### Add compile features
    target_compile_features(${libName} PUBLIC cxx_std_17)
    ### Make target global
    set(${Object} "$<TARGET_OBJECTS:${libName}>" PARENT_SCOPE)
endfunction()
