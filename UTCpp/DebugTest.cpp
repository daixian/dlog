#include "pch.h"
#include "../dlog/Common/MemoryLog.h"
#include "../dlog/Common/Debug.h"
#include "../dlog/dlog.h"
#include  "../dlog/Common/FileHelper.h"

#if defined(_WIN32) || defined(_WIN64)
#pragma comment(lib, "dlog.lib")
#elif defined(__linux__)
#endif


using namespace dxlib;
using namespace std;


TEST(Debug, LogSeverity)
{
    size_t count;

    Debug::GetInst()->clear();
    EXPECT_FALSE(Debug::GetInst()->isInit);//应该还未初始化
    EXPECT_TRUE(Debug::GetInst()->filelogger == nullptr);

    Debug::GetInst()->init();
    EXPECT_TRUE(Debug::GetInst()->isInit);//应该初始化成功
    EXPECT_TRUE(Debug::GetInst()->filelogger != nullptr);

   //关闭整个之后应该没有日志写入
    Debug::GetInst()->isEnable = false;
    Debug::GetInst()->isMemLogEnable = true;

    Debug::GetInst()->LogE("LogE");
    EXPECT_TRUE(MemoryLog::GetInst()->count() == 0);

    //默认不写入LogD
    Debug::GetInst()->isEnable = true;
    Debug::GetInst()->LogD("LogD");
    Debug::GetInst()->LogI("LogI");
    Debug::GetInst()->LogW("LogW");
    Debug::GetInst()->LogE("LogE");

    count = MemoryLog::GetInst()->count();
    EXPECT_TRUE(count == 3) << "当前count" << count;
    string log;
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogI"));
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogW"));
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogE"));

    //打开向内存日志写入LogD
    Debug::GetInst()->isEnable = true;
    Debug::GetInst()->logMemoryThr = spdlog::level::level_enum::debug;
    Debug::GetInst()->LogD("LogD");
    Debug::GetInst()->LogI("LogI");
    Debug::GetInst()->LogW("LogW");
    Debug::GetInst()->LogE("LogE");

    count = MemoryLog::GetInst()->count();
    EXPECT_TRUE(count == 4) << "当前count" << count;
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogD"));
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogI"));
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogW"));
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogE"));


    //打开向内存日志写入LogD
    Debug::GetInst()->isEnable = true;
    Debug::GetInst()->logMemoryThr = spdlog::level::level_enum::err;
    Debug::GetInst()->LogD("LogD");
    Debug::GetInst()->LogI("LogI");
    Debug::GetInst()->LogW("LogW");
    Debug::GetInst()->LogE("LogE");

    count = MemoryLog::GetInst()->count();
    EXPECT_TRUE(count == 1) << "当前count" << count;
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogE"));

    //Debug::GetInst()->LogF("崩溃日志!!");//并不能打印出函数栈
}

TEST(FileHelper, getAppDir)
{
    string dir = FileHelper::getAppDir();
    EXPECT_TRUE(dir.size() > 0);
    dir = FileHelper::getModuleDir();
    EXPECT_TRUE(dir.size() > 0);
}

TEST(Debug, Flush)
{
    Debug::GetInst()->clear();
    Debug::GetInst()->flush();
    Debug::GetInst()->flush();

    Debug::GetInst()->LogI("LogI");
    Debug::GetInst()->LogI("LogI");

    Debug::GetInst()->flush();
    Debug::GetInst()->flush();
}