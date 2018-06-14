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
    std::getline(inputFile, content);

    std::string expected{"42outputFileTest1337"};
    BOOST_CHECK(content.size() == expected.size());
    BOOST_CHECK(expected.compare(content) == 0);
}
