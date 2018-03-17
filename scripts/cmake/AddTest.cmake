#[==[
This function does all the necessary steps to set up a unit test
#]==]

function(addTest TestName)
    ### Add prefix "Test_" to avoid target naming conflicts (for example with benchmarks or other GDL targets)
    set(TestName "Test_${TestName}")

    ### Find single object targets
    if(ARGN)
        foreach(filename ${ARGN})
            string(FIND "${ARGN}" ".cpp" IsCpp)
            if(${IsCpp} EQUAL "-1")
                set(AdditionalLibs
                    "${AdditionalLibs}${ARGN}")
            else()
                singleSourceTargetName(${CMAKE_SOURCE_DIR}/gdl/${filename} target)
                set(AdditionalObjects
                    "${AdditionalObjects};$<TARGET_OBJECTS:${target}>")
            endif()
        endforeach()
    endif()

    ### Create executable
    add_executable(${TestName}
        "${TestName}.cpp"
        ${AdditionalObjects})

    ### Link necessary libs
    target_link_Libraries(${TestName}
        Boost::unit_test_framework
        ${AdditionalLibs}
        )

    ### Add source directory
    target_include_directories(${TestName}
        PUBLIC
            ${PROJECT_SOURCE_DIR})

    ### Add necessary definitions
    target_compile_definitions(${TestName}
        PRIVATE
            -DBOOST_TEST_MODULE=${TestName}
            -DBOOST_TEST_DYN_LINK)

    target_compile_features(${TestName}
        PUBLIC
            cxx_std_17)

    ### Create Test
    string(REPLACE "${CMAKE_SOURCE_DIR}/tests/" ""
        relpath ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "::"
        TestPrefix ${relpath})
    add_test("${TestPrefix}::${TestName}" ${TestName})

endfunction()
