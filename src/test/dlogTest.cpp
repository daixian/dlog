#include "pch.h"
#include "../dlog/Common/MemoryLog.h"
#include "../dlog/Common/Debug.h"
#include "../dlog/dlog.h"
#include "../dlog/Common/FileHelper.h"

#pragma execution_character_set("utf-8")

#if defined(_WIN32) || defined(_WIN64)
#    pragma comment(lib, "dlog.lib")
#elif defined(__linux__)
#endif

using namespace dxlib;
using namespace std;

//注意,在微软的CI服务器上,APPDATA目录没有权限创建,同时不支持中文路径

TEST(dlog, memorylog)
{
    dlog_close();

    dlog_init("log", "memorylog", dlog_init_relative::MODULE);
    dlog_memory_log_enable(true);
    dlog_set_console_thr(dlog_level::err);
    //LogI打印10W条，异步的，只要942毫秒
    //LogW打印10W条，也只要1秒的样子
    for (size_t i = 0; i < 4096; i++) {
        LogW("测试日志%d !", i);
    }

    char msg[512];
    char msgCorr[512];
    for (int i = 0; i < 4096; i++) {
        if (dlog_get_memlog(msg, 0, 512) > 0) {
#if defined(_WIN32) || defined(_WIN64)
            sprintf_s(msgCorr, 512, "测试日志%d !", i); //正确的消息应该是
#elif defined(__linux__)
            snprintf(msgCorr, 512, "测试日志%d !", i); //正确的消息应该是
#endif
            ASSERT_TRUE(strcmp(msg, msgCorr) == 0) << "msg=" << msg; //比对提取的消息是否正确
        }
        else {
            FAIL();
        }
    }
}

TEST(dlog, init_close)
{
    for (size_t i = 0; i < 10; i++) {
        dlog_close();

        //第一次创建
        int res = dlog_init("log", "init_close", dlog_init_relative::MODULE);
        EXPECT_TRUE(res == 0);
        LogI("132");

        //复用
        res = dlog_init("log", "init_close", dlog_init_relative::MODULE, false);
        EXPECT_TRUE(res == 1);
        LogI("132");

        //强制创建,因为重名所以还是复用
        res = dlog_init("log", "init_close", dlog_init_relative::MODULE, true);
        EXPECT_TRUE(res == 3);
        LogI("132");

        //强制创建
        res = dlog_init("log", "init_close2", dlog_init_relative::MODULE, true);
        EXPECT_TRUE(res == 2);
        LogI("132");
    }
    dlog_close();
}

TEST(dlog, logi)
{
    dlog_close();

    //第一次创建
    int res = dlog_init("log", "LogI", dlog_init_relative::MODULE);
    EXPECT_TRUE(res == 0);
    // for (size_t i = 0; i < 5; i++) {
    //     LogI("输出一个日志 %d", i);
    // }
    // LogI("输出一个日志 %s", "这是一段日志，测试%s");
    // string str = "这是一段日志，测试string.c_str()";
    // LogI("输出一个日志 %s", str.c_str());
    // str = "D:/Work/MRSystem/x64/Release/";
    // LogI("GC100CharucoCalib.searchImageInCharucoDir():找到一个charuco图片 %s", str.c_str());

    for (size_t i = 0; i < 100; i++) {
        string fp = "D:/Work/MRSystem/x64/Release/images\\charuco\\dell-dual\\origin\\F3D0001\\20190416-170709.png";
        fp += fp;
        fp += fp;
        fp += fp;
        LogI("GC100CharucoCalib.searchImageInCharucoDir():找到一个charuco图片 %s", fp.c_str());
        LogI_w(L"试试❀❀❀❀❀❀❀❀❀");
    }

    char msg[129];

    for (size_t i = 0; i < sizeof(msg); i++) {
        msg[i] = 'a';
    }
    msg[128] = '\0';
    LogI(msg);
    dlog_close();
}