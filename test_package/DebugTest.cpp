#include "pch.h"
#include "dlog/Common/MemoryLog.h"
#include "dlog/Common/Debug.h"
#include "dlog/dlog.h"
#include "dlog/Common/FileHelper.h"

#include "Poco/LocalDateTime.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeParser.h"

#if defined(_WIN32) || defined(_WIN64)
#    pragma comment(lib, "dlog.lib")
#elif defined(__linux__)
#endif

using namespace dxlib;
using namespace std;

using Poco::DateTime;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::DateTimeParser;
using Poco::LocalDateTime;
/*
年：2019
月：12
日：28
时：2
分：32
秒：15
本周中的第d%天：6
本年中的第d%天：362
儒略日：2.45885e+06
标准格式时间：2019-12-28T02:32:15+08:00
http格式时间：Sat, 28 Dec 2019 02:32:15 +0800
ANSI格式时间：Sat Dec 28 02:32:15 2019
简明格式时间：2019-12-28 02:32:15
*/
TEST(Debug, DateTime)
{
#if defined(_WIN32) || defined(_WIN64)
    system("chcp 65001"); //测试了在windows下有效
#endif

    LocalDateTime now;
    std::cout << "年：" << now.year() << std::endl;
    std::cout << "月：" << now.month() << std::endl;
    std::cout << "日：" << now.day() << std::endl;
    std::cout << "时：" << now.hour() << std::endl;
    std::cout << "分：" << now.minute() << std::endl;
    std::cout << "秒：" << now.second() << std::endl;

    std::cout << "本周中的第d%天：" << now.dayOfWeek() << std::endl;
    std::cout << "本年中的第d%天：" << now.dayOfYear() << std::endl;
    std::cout << "儒略日：" << now.julianDay() << std::endl;

    std::string str = DateTimeFormatter::format(now, DateTimeFormat::ISO8601_FORMAT);
    std::cout << "标准格式时间：" << str << std::endl;

    std::string str_http = DateTimeFormatter::format(now, DateTimeFormat::HTTP_FORMAT);
    std::cout << "http格式时间：" << str_http << std::endl;

    std::string str_asctime = DateTimeFormatter::format(now, DateTimeFormat::ASCTIME_FORMAT);
    std::cout << "ANSI格式时间：" << str_asctime << std::endl;

    std::string str_simple = DateTimeFormatter::format(now, DateTimeFormat::SORTABLE_FORMAT);
    std::cout << "简明格式时间：" << str_simple << std::endl;

    std::cout << "自己的时间格式" << DateTimeFormatter::format(now, "%Y%m%d-%H%M%S") << std::endl;
    DateTime dt;
    int tzd;
    DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, str, dt, tzd);
    dt.makeUTC(tzd);
    LocalDateTime ldt(tzd, dt);
}
//TEST(Debug, LogSeverity)
//{
//    size_t count;
//
//    Debug::GetInst()->clear();
//    EXPECT_FALSE(Debug::GetInst()->isInit); //应该还未初始化
//    EXPECT_TRUE(Debug::GetInst()->filelogger == nullptr);
//
//    Debug::GetInst()->init("log", "dlog-test", INIT_RELATIVE::MODULE);
//    EXPECT_TRUE(Debug::GetInst()->isInit); //应该初始化成功
//    EXPECT_TRUE(Debug::GetInst()->filelogger != nullptr);
//
//    //关闭整个之后应该没有日志写入
//    Debug::GetInst()->isEnable = false;
//    Debug::GetInst()->isMemLogEnable = true;
//    Debug::GetInst()->isConsoleEnable = true;
//
//    Debug::GetInst()->LogE("LogE");
//    EXPECT_TRUE(MemoryLog::GetInst()->count() == 0);
//
//    //默认不写入LogD
//    Debug::GetInst()->isEnable = true;
//    Debug::GetInst()->logConsoleThr = spdlog::level::level_enum::debug;
//    Debug::GetInst()->LogD("LogD");
//    Debug::GetInst()->LogI("LogI");
//    Debug::GetInst()->LogW("LogW");
//    Debug::GetInst()->LogE("LogE");
//
//    count = MemoryLog::GetInst()->count();
//    EXPECT_TRUE(count == 3) << "当前count" << count;
//    string log;
//    MemoryLog::GetInst()->getLog(log);
//    EXPECT_TRUE(log == string("LogI"));
//    MemoryLog::GetInst()->getLog(log);
//    EXPECT_TRUE(log == string("LogW"));
//    MemoryLog::GetInst()->getLog(log);
//    EXPECT_TRUE(log == string("LogE"));
//
//    //打开向内存日志写入LogD
//    Debug::GetInst()->isEnable = true;
//    Debug::GetInst()->logMemoryThr = spdlog::level::level_enum::debug;
//    Debug::GetInst()->LogD("LogD");
//    Debug::GetInst()->LogI("LogI");
//    Debug::GetInst()->LogW("LogW");
//    Debug::GetInst()->LogE("LogE");
//
//    count = MemoryLog::GetInst()->count();
//    EXPECT_TRUE(count == 4) << "当前count" << count;
//    MemoryLog::GetInst()->getLog(log);
//    EXPECT_TRUE(log == string("LogD"));
//    MemoryLog::GetInst()->getLog(log);
//    EXPECT_TRUE(log == string("LogI"));
//    MemoryLog::GetInst()->getLog(log);
//    EXPECT_TRUE(log == string("LogW"));
//    MemoryLog::GetInst()->getLog(log);
//    EXPECT_TRUE(log == string("LogE"));
//
//    //打开向内存日志写入LogD
//    Debug::GetInst()->isEnable = true;
//    Debug::GetInst()->logMemoryThr = spdlog::level::level_enum::err;
//    Debug::GetInst()->LogD("LogD");
//    Debug::GetInst()->LogI("LogI");
//    Debug::GetInst()->LogW("LogW");
//    Debug::GetInst()->LogE("LogE");
//
//    count = MemoryLog::GetInst()->count();
//    EXPECT_TRUE(count == 1) << "当前count" << count;
//    MemoryLog::GetInst()->getLog(log);
//    EXPECT_TRUE(log == string("LogE"));
//
//    //Debug::GetInst()->LogF("崩溃日志!!");//并不能打印出函数栈
//}

TEST(FileHelper, getAppDir)
{
    string dir = FileHelper::getAppDir();
    EXPECT_TRUE(dir.size() > 0);
    dir = FileHelper::getModuleDir();
    EXPECT_TRUE(dir.size() > 0);
}

TEST(Debug, LogMsg)
{
    Debug::GetInst()->clear();
    EXPECT_FALSE(Debug::GetInst()->isInit); //应该还未初始化
    EXPECT_TRUE(Debug::GetInst()->filelogger == nullptr);

    Debug::GetInst()->init("log", "LogMsg", INIT_RELATIVE::MODULE);
    Debug::GetInst()->LogMsg(spdlog::level::level_enum::info, "试一试");
    Debug::GetInst()->LogMsg(spdlog::level::level_enum::info, "试一试");
// 只有在windows上支持这个功能，暂时还是不要使用了
#ifdef SPDLOG_WCHAR_TO_UTF8_SUPPORT
    Debug::GetInst()->LogMsg(spdlog::level::level_enum::info, L"试一试");
    Debug::GetInst()->LogMsg(spdlog::level::level_enum::info, L"试一试");
    Debug::GetInst()->LogMsg(spdlog::level::level_enum::info, L"试一试");
#endif
}
