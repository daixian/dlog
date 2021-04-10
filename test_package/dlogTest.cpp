#include "pch.h"

#define DLOG_UTF8
#include "dlog/dlog.h"

using namespace std;

//注意,在微软的CI服务器上,APPDATA目录没有权限创建,同时不支持中文路径

TEST(dlog, speed)
{
    dlog_close();
    dlog_memory_log_enable(false);
    dlog_console_log_enable(false);
    int res = dlog_init("log", "speed", dlog_init_relative::MODULE);
    ASSERT_TRUE(dlog_is_initialized());

    dlog_set_file_thr(dlog_level::debug);
    int count = 100000; //10万条日志
    clock_t lastTime = clock();

    for (size_t i = 0; i < count; i++) {
        LogI("测试纯文件速度的日志");
    }
    clock_t now = clock();
    float costTime = (float)(now - lastTime) / CLOCKS_PER_SEC;
    dlog_console_log_enable(true);
    LogI("%d条日志的时间=%fs", count, costTime);
    dlog_flush();
    dlog_close();
}

TEST(dlog, speed_format)
{
    dlog_close();
    dlog_memory_log_enable(false);
    dlog_console_log_enable(false);
    int res = dlog_init("log", "speed", dlog_init_relative::MODULE);
    ASSERT_TRUE(dlog_is_initialized());

    dlog_set_file_thr(dlog_level::debug);
    int count = 100000; //10万条日志
    clock_t lastTime = clock();

    for (size_t i = 0; i < count; i++) {
        LogI("测试纯文件速度的日志%d", i);
    }
    clock_t now = clock();
    float costTime = (float)(now - lastTime) / CLOCKS_PER_SEC;
    dlog_console_log_enable(true);
    LogI("%d条带格式化日志的时间=%fs", count, costTime);
    dlog_flush();
    dlog_close();
}

TEST(dlog, memorylog)
{
    dlog_close();
    ASSERT_FALSE(dlog_is_initialized());

    dlog_init("log", "memorylog", dlog_init_relative::MODULE);
    ASSERT_TRUE(dlog_is_initialized());

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
#else
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
        ASSERT_TRUE(dlog_is_initialized());
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

    //文件已经关闭,dlog_get_log_file_path函数返回值为0
    char path[245];
    EXPECT_EQ(dlog_get_log_file_path(path, 245), 0);
}

TEST(dlog, init_close_w)
{
    for (size_t i = 0; i < 10; i++) {
        dlog_close();

        //第一次创建
        int res = dlog_init_wchar_filename(L"log", L"试试中文app", dlog_init_relative::MODULE);
        ASSERT_TRUE(dlog_is_initialized());
        EXPECT_TRUE(res == 0);
        LogI("132");

        //复用
        res = dlog_init_wchar_filename(L"log", L"试试中文app", dlog_init_relative::MODULE, false);
        EXPECT_TRUE(res == 1);
        LogI("132");

        //强制创建,因为重名所以还是复用
        res = dlog_init_wchar_filename(L"log", L"试试中文app", dlog_init_relative::MODULE, true);
        EXPECT_TRUE(res == 3);
        LogI("132");

        //强制创建
        res = dlog_init_wchar_filename(L"log", L"试试中文app_2", dlog_init_relative::MODULE, true);
        EXPECT_TRUE(res == 2);
        LogI("132");
    }
    dlog_close();

    //文件已经关闭,dlog_get_log_file_path函数返回值为0
    char path[245];
    EXPECT_EQ(dlog_get_log_file_path(path, 245), 0);
}

TEST(dlog, logi)
{
    dlog_close();

    //第一次创建
    int res = dlog_init("log", "LogI", dlog_init_relative::MODULE);
    ASSERT_TRUE(dlog_is_initialized());
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
        wLogI(L"试试❀❀❀❀❀❀❀❀❀");
    }

    char msg[129];

    for (size_t i = 0; i < sizeof(msg); i++) {
        msg[i] = 'a';
    }
    msg[128] = '\0';
    LogI(msg);
    dlog_close();
}

TEST(dlog, logD)
{
    dlog_close();

    //第一次创建
    int res = dlog_init("log", "LogD", dlog_init_relative::MODULE);
    ASSERT_TRUE(dlog_is_initialized());
    dlog_set_console_thr(dlog_level::debug);
    dlog_set_file_thr(dlog_level::debug);
    dlog_set_memory_thr(dlog_level::debug);
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
        LogD("GC100CharucoCalib.searchImageInCharucoDir():找到一个charuco图片 %s", fp.c_str());
        wLogD(L"试试❀❀❀❀❀❀❀❀❀");
    }

    char msg[129];

    for (size_t i = 0; i < sizeof(msg); i++) {
        msg[i] = 'a';
    }
    msg[128] = '\0';
    LogI(msg);
    LogD(msg);
    LogW(msg);
    LogE(msg);
    dlog_flush();
    dlog_close();
}

#if WIN32

TEST(dlog, path)
{
    dlog_close();

    //第一次创建
    int res = dlog_init("C:\\ProgramData\\log", "LogD", dlog_init_relative::MODULE);
    ASSERT_TRUE(dlog_is_initialized());
    dlog_set_console_thr(dlog_level::debug);
    dlog_set_file_thr(dlog_level::debug);
    dlog_set_memory_thr(dlog_level::debug);
    EXPECT_TRUE(res == 0);
    char path[245];
    dlog_get_log_dir(path, 245);

    int strcres = strcmp("C:\\ProgramData\\log\\", path);
    EXPECT_TRUE(strcres == 0);

    char msg[129];

    for (size_t i = 0; i < sizeof(msg); i++) {
        msg[i] = 'a';
    }
    msg[128] = '\0';
    LogI(msg);
    LogD(msg);
    LogW(msg);
    LogE(msg);
    dlog_flush();
    dlog_close();
}

TEST(dlog, path2)
{
    dlog_close();

    //第一次创建
    int res = dlog_init("C:\\ProgramData\\log\\", "LogD", dlog_init_relative::MODULE);
    ASSERT_TRUE(dlog_is_initialized());
    dlog_set_console_thr(dlog_level::debug);
    dlog_set_file_thr(dlog_level::debug);
    dlog_set_memory_thr(dlog_level::debug);
    EXPECT_TRUE(res == 0);
    char path[245];
    dlog_get_log_dir(path, 245);

    int strcres = strcmp("C:\\ProgramData\\log\\", path);
    EXPECT_TRUE(strcres == 0);

    char msg[129];

    for (size_t i = 0; i < sizeof(msg); i++) {
        msg[i] = 'a';
    }
    msg[128] = '\0';
    LogI(msg);
    LogD(msg);
    LogW(msg);
    LogE(msg);
    dlog_flush();
    dlog_close();
}

#endif

bool isCallbackIn = false;
void LoggerFun(int level, const char* message)
{
    isCallbackIn = true;
}

TEST(dlog, callback)
{
    dlog_close();

    //第一次创建
    int res = dlog_init("log", "callback", dlog_init_relative::MODULE);
    ASSERT_TRUE(dlog_is_initialized());
    dlog_set_console_thr(dlog_level::debug);
    dlog_set_file_thr(dlog_level::debug);
    dlog_set_logger_function(LoggerFun);

    char msg[129];

    for (size_t i = 0; i < sizeof(msg); i++) {
        msg[i] = 'a';
    }
    msg[128] = '\0';
    LogI(msg);
    LogD(msg);
    LogW(msg);
    LogE(msg);

    ASSERT_TRUE(isCallbackIn);
    dlog_flush();
    dlog_close();
}

TEST(dlog, mayaTest)
{
    dlog_close();

    int res = dlog_init("log", "Maya", dlog_init_relative::MODULE);
    ASSERT_TRUE(dlog_is_initialized());
    dlog_set_console_thr(dlog_level::debug);
    dlog_set_file_thr(dlog_level::debug);

    string text;
    text.push_back(-28);
    text.push_back(-72);
    text.push_back(-102);
    text.push_back(-27);
    text.push_back(-118);
    text.push_back(-95);
    text.push_back(-27);
    text.push_back(-111);
    text.push_back(-104);

    for (size_t i = 0; i < 100; i++) {
        LogI(text.c_str());
    }

    for (size_t i = 0; i < 100; i++) {
        LogMsg(dlog_level::warn, text.c_str());
    }

    dlog_flush();
    dlog_close();
}

TEST(dlog, gbk2utf8)
{
    vector<char> buff(256, 0);
    string utf8 = "中文热带水果地方改";
    string gbk;
    dlog_convert_utf8_to_gbk(utf8.c_str(), buff.data(), buff.size());
    gbk = buff.data();

    dlog_convert_gbk_to_utf8(gbk.c_str(), buff.data(), buff.size());
    string utf8_2 = buff.data();

    ASSERT_TRUE(utf8 == utf8_2);
}