#include "gtest/gtest.h"

#if DLOG_STATIC

#    include "dlog/Common/MemoryLog.h"
#    include "dlog/Common/Debug.h"
#    include "dlog/dlog.h"
#    include "dlog/Common/FileHelper.h"

using namespace dlog;
using namespace std;

//测试MemoryLog类
TEST(MemoryLog, ManyTest)
{
    dlog_close();
    for (size_t i = 0; i < 1024; i++) {
        MemoryLog::GetInst()->addLog(std::to_string(i));
    }
    ASSERT_TRUE(MemoryLog::GetInst()->count() == 1024);

    //检查clear()函数
    MemoryLog::GetInst()->clear();
    ASSERT_TRUE(MemoryLog::GetInst()->count() == 0);

    //执行1024*10次
    for (size_t runCount = 0; runCount < 100; runCount++) {
        //向日志里写1024条
        for (size_t i = 0; i < 1024; i++) {
            MemoryLog::GetInst()->addLog(std::to_string(i));
        }

        //检查取出来的和写进去的是否一致
        string log;
        for (size_t i = 0; i < 1024; i++) {
            bool res = MemoryLog::GetInst()->getLog(log);
            ASSERT_TRUE(res); //1024条日志应该都成功
            ASSERT_TRUE(log == std::to_string(i));
        }
        ASSERT_TRUE(MemoryLog::GetInst()->count() == 0);
    }

    MemoryLog::GetInst()->clear();
}

#endif
