#include <boost/test/unit_test.hpp>

#include "gdl/base/outputFile.h"

#include <fstream>

using namespace GDL;

BOOST_AUTO_TEST_CASE(outputFile_Cant_Open_File)
{
    BOOST_CHECK_THROW(OutputFile{"some/non/existing/path/"}, Exception);
}


BOOST_AUTO_TEST_CASE(outputFile_Write)
{
    std::string fileName{"outputFileTest"};
    {
        OutputFile file{fileName};
        file.Write(42, fileName, 1337);
    }

    std::ifstream inputFile;
    inputFile.open(fileName);

    // File is accessable
    BOOST_CHECK(inputFile.is_open());

    // Check content
    std::string content;
    if (inputFile.is_open())
        std::getline(inputFile, content);

    std::string expected{"42outputFileTest1337"};
    BOOST_CHECK(content.size() == expected.size());
    BOOST_CHECK(expected.compare(content) == 0);
    inputFile.close();
}

BOOST_AUTO_TEST_CASE(outputFile_Multi_open)
{
    std::ifstream inputFile;
    std::string fileName{"outputFileTest"};
    OutputFile file{fileName};
    file.Write(1337);
    file.Open(fileName); // close and reopen file -> second open clears the file content

    inputFile.open(fileName);

    // File is accessable
    BOOST_CHECK(inputFile.is_open());

    // Check content
    std::string content;
    std::getline(inputFile, content);
    BOOST_CHECK(content.size() == 0);


    inputFile.close();
}
