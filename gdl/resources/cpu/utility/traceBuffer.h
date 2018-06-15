#pragma once

#include "gdl/GDLTypedefs.h"
#include "gdl/base/outputFile.h"
#include "gdl/base/timer.h"

#include <array>
#include <atomic>
#include <cassert>
#include <string>
#include <tuple>
#include <thread>


namespace GDL
{
template <U32 _BufferSize>
class TraceBuffer
{

    //! @brief This class describes an event
    struct EventLog
    {
        std::thread::id mThreadId;
        U32 mTimestamp;
        std::atomic_bool mWrittenToFile;
        std::string mEventMessage;


        EventLog()
            : mThreadId{std::this_thread::get_id()}
            , mTimestamp{0}
            , mWrittenToFile{true}
            , mEventMessage{""}
        {
        }


        EventLog(std::thread::id threadId, U32 timestamp, std::string eventMessage, bool writtenToFile)
            : mThreadId{threadId}
            , mTimestamp{timestamp}
            , mWrittenToFile{writtenToFile}
            , mEventMessage{eventMessage}
        {
        }

        EventLog(const EventLog&) = delete;
        EventLog(EventLog&&) = delete;
        EventLog& operator=(const EventLog&) = delete;

        EventLog& operator=(EventLog&& other)
        {
            mThreadId = other.mThreadId;
            mTimestamp = other.mTimestamp;
            mWrittenToFile = other.mWrittenToFile.load();
            mEventMessage = std::move(other.mEventMessage);
            return *this;
        }

        ~EventLog() = default;
    };


    std::atomic_int32_t mIndex;
    std::atomic_bool mWriteToLogfile;
    std::array<EventLog, _BufferSize> mLog;
    Timer mTimer;
    OutputFile mLogFile;
    //    std::thread mWriterThread;


    TraceBuffer()
        : mIndex{0}
        , mWriteToLogfile{false}
    {
    }

public:
    TraceBuffer(const TraceBuffer&) = delete;
    TraceBuffer(TraceBuffer&&) = delete;
    TraceBuffer& operator=(const TraceBuffer&) = delete;
    TraceBuffer& operator=(TraceBuffer&&) = delete;
    ~TraceBuffer() = default;

    //! @brief Direct access operator to get a specific event log
    //! @param index: Index of the desired event log
    //! @return Requested event log
    const EventLog& operator[](I32 index) const
    {
        assert(index > -1 && index < static_cast<I32>(_BufferSize));
        return mLog[index];
    }

    //! @brief Gets the singleton intance of the trace buffer
    //! @return Intance of the trace buffer
    static TraceBuffer& Instance()
    {
        static TraceBuffer tb;
        return tb;
    }


    //! @brief Returns the buffer size
    //! @return Buffer size
    constexpr U32 GetBufferSize() const
    {
        return _BufferSize;
    }


    //! @brief Returns the current array index of the trace buffer that would be written to
    //! @return Current array index of the trace buffer that would be written to
    I32 GetCurrentIndex() const
    {
        return mIndex;
    }


    //! @brief Stores a new event in the buffer
    //! @param eventMessage: Message for that describes the event
    void LogEvent(const std::string& eventMessage)
    {
        I32 index = GetNewIndex();
        mLog[index] = EventLog(std::this_thread::get_id(), mTimer.GetMilliseconds(), eventMessage, false);
    }


    //! @brief Opens an output file and starts a writer thread
    //! @param fileName: Name of the output file
    void OpenLogFile(std::string fileName)
    {
        //        if (mWriterThread.joinable())
        //        {
        //            mWriteToLogfile = false;
        //            mWriterThread.join();
        //        }

        //        mWriterThread.
    }

private:
    //! @brief Gets the desired index that should be reserved for the current thread
    //! @param expectedValue: Expected value of the atomic index
    //! @return Index that should be reserved for the current thread
    I32 GetDesiredIndex(I32 expectedValue) const
    {
        if (expectedValue == _BufferSize - 1)
            return 0;
        return expectedValue + 1;
    }

    //! @brief Gets the index that is reserved for the current thread
    //! @return Index that is reserved for the current thread
    I32 GetNewIndex()
    {
        I32 newIndex = mIndex.load();

        while (!mIndex.compare_exchange_weak(newIndex, GetDesiredIndex(newIndex)))
            ;
        return newIndex;
    }
};
}
