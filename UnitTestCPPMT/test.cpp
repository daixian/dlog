#include "pch.h"
#include "../dlog/Common/MemoryLog.h"
#include "../dlog/Common/Debug.h"
#include "../dlog/dlog.h"

using namespace dxlib;
using namespace std;

//测试MemoryLog类
TEST(MemoryLog, ManyTest)
{
    for (size_t i = 0; i < 1024; i++) {
        MemoryLog::GetInst()->addLog(std::to_string(i));
    }
    EXPECT_TRUE(MemoryLog::GetInst()->count() == 1024);

    //检查clear()函数
    MemoryLog::GetInst()->clear();
    EXPECT_TRUE(MemoryLog::GetInst()->count() == 0);

    //执行1024*10次
    for (size_t runCount = 0; runCount < 1024 * 10; runCount++) {
        //向日志里写1024条
        for (size_t i = 0; i < 1024; i++) {
            MemoryLog::GetInst()->addLog(std::to_string(i));
        }

        //检查取出来的和写进去的是否一致
        string log;
        for (size_t i = 0; i < 1024; i++) {
            bool res = MemoryLog::GetInst()->getLog(log);
            EXPECT_TRUE(res);//1024条日志应该都成功
            EXPECT_TRUE(log == std::to_string(i));
        }
        EXPECT_TRUE(MemoryLog::GetInst()->count() == 0);
    }
}

TEST(Debug, LogSeverity)
{
    int count;

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
    EXPECT_TRUE(count == 3)<< "当前count" << count;
    string log;
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogI"));
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogW"));
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogE"));

    //打开向内存日志写入LogD
    Debug::GetInst()->isEnable = true; 
    Debug::GetInst()->logMemoryThr = -1;
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
    Debug::GetInst()->logMemoryThr = DLOG_ERROR;
    Debug::GetInst()->LogD("LogD");
    Debug::GetInst()->LogI("LogI");
    Debug::GetInst()->LogW("LogW");
    Debug::GetInst()->LogE("LogE");

    count = MemoryLog::GetInst()->count();
    EXPECT_TRUE(count == 1) << "当前count" << count;
    MemoryLog::GetInst()->getLog(log);
    EXPECT_TRUE(log == string("LogE"));
}


TEST(dlog, memorylog) {
    dlog_init("D:\\临时测试\\log", "测试日志");
    dlog_memory_log_enable(true);

    //LogI打印10W条，异步的，只要942毫秒
    //LogW打印10W条，也只要1秒的样子
    for (size_t i = 0; i < 4096; i++) {
        LogW("测试日志%d !", i);
    }

    char msg[512];
    char msgCorr[512];
    for (size_t i = 0; i < 4096; i++) {
        if (dlog_get_memlog(msg, 0, 512) > 0) {
            sprintf(msgCorr, "测试日志%d !", i);//正确的消息应该是
            EXPECT_TRUE(strcmp(msg, msgCorr) == 0) << "msg=" << msg;//比对提取的消息是否正确
        }
        else {
            FAIL(); 
        }

    }

}