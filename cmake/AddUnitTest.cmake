#[==[
This function does all the necessary steps to set up a unit test
#]==]

# This function creates a unit test
function(get_target_from_source Source Target)
    string(REPLACE "${CMAKE_SOURCE_DIR}/src/" "" libName ${Source})
    string(REPLACE ".cpp" "" libName ${libName})
    string(REPLACE "/" "." libName ${libName})
    set(${Target} ${libName} PARENT_SCOPE)
endfunction()

function(add_unit_test ClassName)
    MESSAGE(STATUS ${ClassName})

    # get the relative path to the test directory
    string(REPLACE "${CMAKE_SOURCE_DIR}/tests/" ""
        relpath ${CMAKE_CURRENT_SOURCE_DIR})


    if(ARGN)
        foreach(filename ${ARGN})
            get_target_from_source(${CMAKE_SOURCE_DIR}/src/${filename} target)
            #message("$target: ${target}")
            set(AdditionalObjects
                "${AdditionalObjects};$<TARGET_OBJECTS:${target}>")
            #message("$additional: ${AdditionalObjects}")
        endforeach()
    endif()
endfunction()
