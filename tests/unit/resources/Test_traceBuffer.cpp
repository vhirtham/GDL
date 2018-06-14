#include <boost/test/unit_test.hpp>

#include "gdl/resources/cpu/utility/traceBuffer.h"


#include <iostream>
#include <vector>


using namespace GDL;


//! @brief Checks if the trace buffer works as expected with only a single thread
BOOST_AUTO_TEST_CASE(traceBuffer_Single)
{
    TraceBuffer<100>& tb = TraceBuffer<100>::Instance();
    BOOST_CHECK(tb.GetCurrentIndex() == 0);
    for (I32 i = 0; i < tb.GetBufferSize() - 2; ++i)
        tb.LogEvent("event");
    BOOST_CHECK(tb.GetCurrentIndex() == tb.GetBufferSize() - 2);
    tb.LogEvent("event");
    BOOST_CHECK(tb.GetCurrentIndex() == tb.GetBufferSize() - 1);
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


    BOOST_CHECK(tb.GetCurrentIndex() == totalNumberOfEvents % tb.GetBufferSize());
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

    for (U32 i = 0; i < totalNumberOfEvents; ++i)
    {
        BOOST_CHECK(tb[i].mEventMessage.size() == 1);
        BOOST_CHECK(tb[i].mEventMessage[0] == 'e');
    }
    BOOST_CHECK(tb[totalNumberOfEvents].mEventMessage.size() == 0);
}
