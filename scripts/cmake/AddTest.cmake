#[==[
This function does all the necessary steps to set up a unit test
#]==]

function(addTest TestName)
    ### Add prefix Test to avoid target naming conflicts (for example with benchmarks or other GDL targets)
    set(TestName "Test_${TestName}")

    ### Find single object targets
    if(ARGN)
        foreach(filename ${ARGN})
            singleSourceTargetName(${CMAKE_SOURCE_DIR}/src/${filename} target)
            set(AdditionalObjects
                "${AdditionalObjects};$<TARGET_OBJECTS:${target}>")
        endforeach()
    endif()

    ### Create executable
    add_executable(${TestName}
        "${TestName}.cpp"
        ${AdditionalObjects})

    ### Link necessary libs
    target_link_Libraries(${TestName}
        Boost::unit_test_framework)

    ### Add source directory
    target_include_directories(${TestName}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/src)

    ### Create Test
    string(REPLACE "${CMAKE_SOURCE_DIR}/tests/" ""
        relpath ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "::"
        TestPrefix ${relpath})
    add_test("${TestPrefix}::${TestName}" ${TestName})

endfunction()
