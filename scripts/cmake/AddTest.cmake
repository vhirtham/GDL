#[==[
This function does all the necessary steps to set up a unit test
#]==]

function(addTest TestName)
    ### Add prefix "Test_" to avoid target naming conflicts (for example with benchmarks or other GDL targets)
    set(TestName "Test_${TestName}")

    ### Separate sources from libs
    if(ARGN)
        foreach(input ${ARGN})
            string(FIND "${input}" ".cpp" IsCpp)
            if(${IsCpp} EQUAL "-1")
                set(AdditionalLibs
                    "${AdditionalLibs};${input}")
            else()
                set(AdditionalSources
                    "${AdditionalSources};${GDL_SOURCE_DIR}/gdl/${input}")
            endif()
        endforeach()
    endif()

    ### Create executable
    add_executable(${TestName}
        "${TestName}.cpp"
        ${AdditionalSources})

    ### Link necessary libs
    target_link_Libraries(${TestName}
        Boost::unit_test_framework
        ${AdditionalLibs}
        )

    TargetDefaultBuildSetup(${TestName})

    ### Add necessary definitions
    target_compile_definitions(${TestName}
        PRIVATE
            -DBOOST_TEST_MODULE=${TestName}
            -DBOOST_TEST_DYN_LINK)


    ### Create Test
    string(REPLACE "${GDL_SOURCE_DIR}/tests/" ""
        relpath ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "::"
        TestPrefix ${relpath})
    add_test("${TestPrefix}::${TestName}" ${TestName})

endfunction()
