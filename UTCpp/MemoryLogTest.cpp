#include "pch.h"
#include "../dlog/Common/MemoryLog.h"
#include "../dlog/Common/Debug.h"
#include "../dlog/dlog.h"
#include  "../dlog/Common/FileHelper.h"

#pragma comment(lib, "dlog.lib")

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