#include <boost/test/unit_test.hpp>

#include "gdl/resources/cpu/utility/traceBuffer.h"


#include <iostream>
#include <fstream>
#include <vector>


using namespace GDL;


//! @brief Checks if the trace buffer works as expected with only a single thread
BOOST_AUTO_TEST_CASE(traceBuffer_Single)
{
    TraceBuffer<100>& tb = TraceBuffer<100>::Instance();
    BOOST_CHECK(tb.GetCurrentIndex() == 0);
    for (U32 i = 0; i < tb.GetBufferSize() - 2; ++i)
        tb.LogEvent("event");
    BOOST_CHECK(tb.GetCurrentIndex() == static_cast<I32>(tb.GetBufferSize()) - 2);
    tb.LogEvent("event");
    BOOST_CHECK(tb.GetCurrentIndex() == static_cast<I32>(tb.GetBufferSize()) - 1);
    tb.LogEvent("event");
    BOOST_CHECK(tb.GetCurrentIndex() == 0);
    tb.LogEvent("event");
    BOOST_CHECK(tb.GetCurrentIndex() == 1);
}


//! @brief Checks if the trace buffer works as expected with multiple threads. The number of writes to the buffer is
//! checked by comparing the current index with the expected index which can be calculated from the given number of log
//! events.
//! @remark This does not ensure that 2 threads will never get the same index which would result in a race condition. It
//! just ensures that the index is always increased. The race condition is checked in the following test.
BOOST_AUTO_TEST_CASE(traceBuffer_Multi_Index_Correct)
{
    constexpr U32 bufferSize = 200;
    TraceBuffer<bufferSize>& tb = TraceBuffer<bufferSize>::Instance();

    std::array<U32, 4> numLogsForThread = {{158, 312, 88, 175}};
    std::vector<std::thread> threads;

    U32 totalNumberOfEvents = 0;

    for (U32 i = 0; i < numLogsForThread.size(); ++i)
    {
        threads.emplace_back([&numLogsForThread, i]() {
            TraceBuffer<bufferSize>& tb = TraceBuffer<bufferSize>::Instance();
            for (U32 j = 0; j < numLogsForThread[i]; ++j)
            {
                tb.LogEvent("event");
            }
        });
        totalNumberOfEvents += numLogsForThread[i];
    }



    for (U32 i = 0; i < numLogsForThread.size(); ++i)
        threads[i].join();


    BOOST_CHECK(tb.GetCurrentIndex() == static_cast<I32>(totalNumberOfEvents % tb.GetBufferSize()));
}


//! @brief Checks, that every index is only passed to a single thread.
BOOST_AUTO_TEST_CASE(traceBuffer_Multi_Unique_Indices)
{
    constexpr U32 bufferSize = 1000;
    TraceBuffer<bufferSize>& tb = TraceBuffer<bufferSize>::Instance();

    std::array<U32, 4> numLogsForThread = {{158, 312, 88, 175}};
    std::vector<std::thread> threads;

    U32 totalNumberOfEvents = 0;

    for (U32 i = 0; i < numLogsForThread.size(); ++i)
    {
        threads.emplace_back([&numLogsForThread, i]() {
            TraceBuffer<bufferSize>& tb = TraceBuffer<bufferSize>::Instance();
            for (U32 j = 0; j < numLogsForThread[i]; ++j)
            {
                tb.LogEvent("e");
            }
        });
        totalNumberOfEvents += numLogsForThread[i];
    }
    assert(totalNumberOfEvents < bufferSize);



    for (U32 i = 0; i < numLogsForThread.size(); ++i)
        threads[i].join();

    for (I32 i = 0; i < static_cast<I32>(totalNumberOfEvents); ++i)
    {
        BOOST_CHECK(tb[i].mEventMessage.size() == 1);
        BOOST_CHECK(tb[i].mEventMessage[0] == 'e');
    }
    BOOST_CHECK(tb[totalNumberOfEvents].mEventMessage.size() == 0);
}

//! @brief Checks if log files are written correctly.
BOOST_AUTO_TEST_CASE(traceBuffer_LogFile_Write)
{
    constexpr U32 numThreads = 4;
    constexpr U32 bufferSize = 6;
    std::string fileName{"traceBufferLog"};
    TraceBuffer<bufferSize>& tb = TraceBuffer<bufferSize>::Instance();
    tb.OpenLogFile(fileName);

    // Give writer thread some time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(10));


    std::vector<std::thread> threads;

    for (U32 i = 0; i < numThreads; ++i)
    {
        threads.emplace_back([]() {
            TraceBuffer<bufferSize>& tb_thread = TraceBuffer<bufferSize>::Instance();
            tb_thread.LogEvent("e");
        });
    }
    for (U32 i = 0; i < numThreads; ++i)
        threads[i].join();
    threads.clear();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    for (U32 i = 0; i < numThreads; ++i)
    {
        threads.emplace_back([]() {
            TraceBuffer<bufferSize>& tb_thread = TraceBuffer<bufferSize>::Instance();
            tb_thread.LogEvent("e");
        });
    }
    for (U32 i = 0; i < numThreads; ++i)
        threads[i].join();
    threads.clear();

    // Wait for file to be written
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // Check File content
    std::ifstream inputFile;
    inputFile.open(fileName);

    // File is accessable
    BOOST_CHECK(inputFile.is_open());

    U32 lineCounter = 0;
    if (inputFile.is_open())
        while (!inputFile.eof())
        {
            std::string line;
            std::getline(inputFile, line);
            ++lineCounter;
        }
    BOOST_CHECK(lineCounter == 9); // last line is empty but not eof
}

template <class _TraceBuffer>
void FillTraceBufferWithEvents(_TraceBuffer& tb, U32 numEvents)
{
    for (U32 i = 0; i < numEvents; ++i)
        tb.LogEvent("e");
}

//! @brief Checks if an exception is thrown if the trace buffer is filled faster than the logfile can be written
BOOST_AUTO_TEST_CASE(traceBuffer_Buffer_Overflow)
{
    constexpr U32 bufferSize = 3;
    std::string fileName{"traceBufferLog"};
    TraceBuffer<bufferSize>& tb = TraceBuffer<bufferSize>::Instance();
    tb.OpenLogFile(fileName);
    BOOST_CHECK_THROW(FillTraceBufferWithEvents(tb, 10000), Exception);
}
