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

    ### Separate passed inputs
    SeparateInputs("${ARGN}")

    ### Add inputs which are relevant for tests
    set(Sources
        ${TestFileName}.cpp
        ${Sources})

    set(Libraries
        Boost::unit_test_framework
        ${Libraries})

    set(Definitions
        PRIVATE
            -DBOOST_TEST_MODULE=${TestName}
            -DBOOST_TEST_DYN_LINK
        ${Definitions})


    AddExecutable(${TestName} "${Sources}" "${Libraries}" "${Definitions}")
    TargetDefaultBuildSetup(${TestName})


    ### Create Test
    string(REPLACE "${GDL_ROOT_DIR}/test/" ""
        Relpath ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "/" "::"
        TestPrefix ${Relpath})
    add_test("${TestPrefix}::${TestNameWithoutPrefix}" ${TestName})

endfunction()
