#pragma once

#include "gdl/base/fundamentalTypes.h"
#include "gdl/base/outputFile.h"
#include "gdl/base/time.h"
#include "gdl/base/timer.h"
#include "gdl/resources/cpu/thread.h"

#include <array>
#include <atomic>
#include <cassert>
#include <memory>
#include <string>
#include <tuple>
#include <thread>

#include <iostream>
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
        std::atomic_bool mWriteToFile;
        std::string mEventMessage;


        EventLog()
            : mThreadId{std::this_thread::get_id()}
            , mTimestamp{0}
            , mWriteToFile{false}
            , mEventMessage{""}
        {
        }


        EventLog(std::thread::id threadId, U32 timestamp, std::string eventMessage, bool writeToFile)
            : mThreadId{threadId}
            , mTimestamp{timestamp}
            , mWriteToFile{writeToFile}
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
            mWriteToFile = other.mWriteToFile.load();
            mEventMessage = std::move(other.mEventMessage);
            return *this;
        }

        ~EventLog() = default;
    };


    std::atomic_int32_t mIndex;
    std::atomic_bool mWriteToLogfile;
    std::array<EventLog, _BufferSize> mLog;
    Timer mTimer;
    std::unique_ptr<Thread> mWriterThread;


    TraceBuffer()
        : mIndex{0}
        , mWriteToLogfile{false}
        , mWriterThread{nullptr}
    {
    }

public:
    TraceBuffer(const TraceBuffer&) = delete;
    TraceBuffer(TraceBuffer&&) = delete;
    TraceBuffer& operator=(const TraceBuffer&) = delete;
    TraceBuffer& operator=(TraceBuffer&&) = delete;

    //! Dtor - Stops the thread which is writing to the log file
    ~TraceBuffer()
    {
        CloseLogFile();
    }

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
        I32 index = GetReservedIndex();
        if (mLog[index].mWriteToFile)
            throw Exception(__PRETTY_FUNCTION__, "Trace buffer full! Messages are logged faster as they are written to "
                                                 "file. Adjust buffer size or delay events.");
        mLog[index] = EventLog(std::this_thread::get_id(), mTimer.GetElapsedTime<Milliseconds>().count(), eventMessage,
                               mWriteToLogfile);
    }

    //! @brief Closes the log file
    void CloseLogFile()
    {
        // send exit signal for writer loop
        mWriteToLogfile = false;
        // Waits until the thread is joined before setting the loop variable back to true
        mWriterThread.reset(nullptr);
    }



    //! @brief Opens an output file and starts a writer thread
    //! @param fileName: Name of the output file
    void OpenLogFile(std::string fileName)
    {
        CloseLogFile();

        mWriteToLogfile = true;

        mWriterThread.reset(new Thread{[fileName, this]() {
            OutputFile logFile{fileName};

            I32 index = mIndex.load();

            while (mWriteToLogfile || mLog[index].mWriteToFile)
            {
                EventLog& logMessage = mLog[index];
                if (logMessage.mWriteToFile)
                {
                    logFile.Write(logMessage.mThreadId, ", ", logMessage.mTimestamp, ", ", logMessage.mEventMessage,
                                  "\n");
                    logMessage.mWriteToFile = false;
                    index = GetNextIndex(index);
                    continue;
                }
                std::this_thread::yield();
            }
        }});


        //        mWriterThread.
    }



private:
    //! @brief Gets the next index, based on the passed value
    //! @param currentIndex: Current index
    //! @return Next index, based on the passed value
    I32 GetNextIndex(I32 currentIndex) const
    {
        if (currentIndex == _BufferSize - 1)
            return 0;
        return currentIndex + 1;
    }

    //! @brief Gets the index that is reserved for the current thread
    //! @return Index that is reserved for the current thread
    I32 GetReservedIndex()
    {
        I32 reservedIndex = mIndex.load();

        while (!mIndex.compare_exchange_weak(reservedIndex, GetNextIndex(reservedIndex)))
            ;
        return reservedIndex;
    }
};
}
