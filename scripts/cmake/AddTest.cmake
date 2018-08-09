#[==[
This function does all the necessary steps to set up a unit test
#]==]

function(addTest TestName)
    set(TestNameWithoutPrefix ${TestName})
    ### Add prefix "Test_" to avoid target naming conflicts (for example with benchmarks or other GDL targets)
    set(TestName "Test_${TestName}")


    ### Generate file name (It is basically the test name but everything after an "-" will be removed.
    ### This makes it possible to use the same test file for different setups (for example by using defines).
    set(TestFileName "${TestName}")
    string(FIND "${TestFileName}" "-" findHyphenResult)
    if(NOT ${findHyphenResult} EQUAL "-1")
        string(SUBSTRING ${TestFileName} 0 ${findHyphenResult} TestFileName)
    endif()

    ### Separate sources, libs and definitions
    if(ARGN)
        foreach(input ${ARGN})
            string(FIND "${input}" ".cpp" findCppResult)
            if(${findCppResult} EQUAL "-1")
                string(FIND ${input} "-" findDefResult)
                if(NOT ${findDefResult} EQUAL "-1")
                    set(AdditionalDefines
                        "${AdditionalDefines};${input}")
                else()
                    set(AdditionalLibs
                        "${AdditionalLibs};${input}")
                endif()
            else()
                set(AdditionalSources
                    "${AdditionalSources};${GDL_SOURCE_DIR}/gdl/${input}")
            endif()
        endforeach()
    endif()

    ### Create executable
    add_executable(${TestName}
        "${TestFileName}.cpp"
        ${AdditionalSources})

    ### Link necessary libs
    target_link_Libraries(${TestName}
        Boost::unit_test_framework
        ${AdditionalLibs}
        )

    target_compile_definitions(${TestName}
        PUBLIC
            ${AdditionalDefines})

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
    add_test("${TestPrefix}::${TestNameWithoutPrefix}" ${TestName})

endfunction()
