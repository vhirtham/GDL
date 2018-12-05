#pragma once


#include "gdl/base/exception.h"

#include <string>
#include <fstream>

namespace GDL
{

//! @brief Simple class to write data to the hard drive
class OutputFile
{

    std::ofstream mFile;

public:
    //! @brief Ctor that creates/overwrites the specified file
    //! @param fileName: Name of the file that should be written to
    OutputFile(const std::string& fileName)
    {
        Open(fileName);
    }

    OutputFile() = default;
    OutputFile(const OutputFile&) = delete;
    OutputFile(OutputFile&&) = delete;
    OutputFile& operator=(const OutputFile&) = delete;
    OutputFile& operator=(OutputFile&&) = delete;


    //! @brief Dtor which closes the opened file
    ~OutputFile()
    {
        Close();
    }

    //! @brief Opens the output file
    //! @param fileName: Name of the file that should be written to
    void Open(std::string fileName)
    {
        if (IsOpen())
            Close();
        mFile.open(fileName, std::ofstream::out | std::ofstream::trunc);
        if (!IsOpen())
            throw Exception(__PRETTY_FUNCTION__, "Can't open output file \"" + fileName + "\" !");
    }


    //! @brief Closes the output file
    void Close()
    {
        if (IsOpen())
            mFile.close();
    }

    //! @brief Returns if a file is opened
    //! @return TRUE if a file is opened, FALSE otherwise
    bool IsOpen() const
    {
        return mFile.is_open();
    }

    //! @brief Writes data to the file
    //! @tparam _type: Data type of the passed data
    //! @param data: Data that should be written
    template <typename _type>
    OutputFile& Write(const _type& data)
    {
        mFile << data;
        mFile.flush();
        return *this;
    }

    //! @brief Writes data to the file
    //! @tparam _type: Data type of the passed data
    //! @tparam _args: Parameter pack for additional parameters
    //! @param data: Data that should be written
    //! @param args: Additional data
    template <typename _type, typename... _args>
    OutputFile& Write(const _type& data, const _args&... args)
    {
        mFile << data;
        Write(args...);
        return *this;
    }
};
} // namespace GDL
