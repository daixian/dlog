#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/DirectoryIterator.h>
#include <Poco/Format.h>
#include "Poco/Glob.h"
#include "Poco/FileStream.h"
#include "Poco/BinaryReader.h"

#include "gtest/gtest.h"

using namespace Poco;

TEST(Poco, PathTest)
{

    Path inputDir("C:\\ProgramData\\log\\");
    ASSERT_TRUE(inputDir.isAbsolute());

    Path inputDir2("\\log");
    ASSERT_TRUE(inputDir2.isAbsolute());

    Path inputDir3("C:\\ProgramData\\log");
    ASSERT_TRUE(inputDir3.isAbsolute());

    Path inputDir4("log");
    ASSERT_TRUE(!inputDir4.isAbsolute());
}