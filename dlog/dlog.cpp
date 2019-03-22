﻿// dlog.cpp : 定义 DLL 应用程序的导出函数。
//
#include "dlog.h"

#include "./Common/Debug.h"
#include "./Common/MemoryLog.h"
#include "./Common/FileHelper.h"

using namespace dxlib;

///-------------------------------------------------------------------------------------------------
/// <summary> 去得到一个全局指针的值，如果c++的值和c#的值相等那么应该是共用log模块了. </summary>
///
/// <remarks> Dx, 2018/11/2. </remarks>
///
/// <returns>
/// 直接返回全局的指针
/// </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void* __stdcall dlog_global_ptr()
{
    return Debug::GetInst();
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 模块初始化,日志文件夹路径可以使用绝对目录也可以使用相对目录(第三个参数进行相对位置的设置),
/// 如果使用相对目录,那么程序会将它理解为相对模块目录,路径例如 char* logDir = "\\log",char* program = "dlog".
/// isForceInit如果为false，那么就可以不强制初始化模块，理论上整个程序都共用一个日志.
/// 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2,
/// 如果强制重设但是失败还是复用了那么返回3.
/// </summary>
///
/// <remarks>
/// DLOG_INIT_RELATIVE_APPDATA: 相对于AppData文件夹.
/// DLOG_INIT_RELATIVE_MODULE: 相对于dll文件自身文件夹.
/// Dx, 2018/4/22.
/// </remarks>
///
/// <param name="logDir">      [in]日志文件夹路径名（相对模块目录）. </param>
/// <param name="program">     [in]程序名. </param>
/// <param name="dir_relatvie">(Optional)相对路径的相对文件夹位置. </param>
/// <param name="isForceInit"> (Optional) 如果为false，那么就可以不强制初始化模块，理论上整个程序都共用一个日志. </param>
///
/// <returns>
/// 如果之前未被初始化返回0,如果成功复用那么就返回1,如果强制重设成功那么返回2,
/// 如果强制重设但是失败还是复用了那么返回3.
/// </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_init(const char* logDir, const char* program, int dir_relatvie, bool isForceInit)
{
    if (isForceInit == false) {
        if (!Debug::GetInst()->isInit) {
            Debug::GetInst()->init(program, logDir, (INIT_RELATIVE)dir_relatvie);
            return 0; //第一次初始化
        }
        return 1; //成功复用
    }
    else {
        //如果设置了强制初始化
        if (Debug::GetInst()->programName.compare(program) != 0 && //如果两次设置的程序名不一致，那么才删除
            strcmp(program, "dlog") != 0) {                        //同时第二次设置的这个程序名不能等于默认名字
            Debug::GetInst()->clear();
            Debug::GetInst()->init(program, logDir, (INIT_RELATIVE)dir_relatvie);
            return 2; //强制重设了一次glog
        }
        return 3; //强制重设了一次glog
    }
}

///-------------------------------------------------------------------------------------------------
/// <summary> 关闭模块. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_close()
{
    Debug::GetInst()->clear();
    return 0;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 设置整个log使能. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_enable(bool enable)
{
    Debug::GetInst()->isEnable = enable;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 设置控制台log是否使能. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_console_log_enable(bool enable)
{
    Debug::GetInst()->setIsConsoleEnable(enable);
}

///-------------------------------------------------------------------------------------------------
/// <summary> 设置Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会写入. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_usual_thr(int usualThr)
{
    Debug::GetInst()->logUsualThr = usualThr;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到Dlog的常规日志（非内存日志）门限,大于等于该优先级的日志都会写入. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_usual_thr()
{
    return Debug::GetInst()->logUsualThr;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 设置Dlog的内存日志门限,大于等于该优先级的日志都会写入. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_memory_thr(int memoryThr)
{
    Debug::GetInst()->logMemoryThr = memoryThr;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到Dlog的内存日志门限,大于等于该优先级的日志都会写入. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_memory_thr()
{
    return Debug::GetInst()->logMemoryThr;
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 设置大于等于某一级日志输出到控制台. 参数有DLOG_INFO,DLOG_WARNING,DLOG_ERROR
/// </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="LogSeverity"> 大于等于这一级的日志都会输出到控制台. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_console_thr(int LogSeverity)
{
    Debug::GetInst()->logConsoleThr = LogSeverity; // 当日志级别大于等于此级别时，自动将此日志输出到标准错误(终端窗口)中
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到Dlog的控制台日志门限,大于等于该优先级的日志都会写入. </summary>
///
/// <remarks> Dx, 2018/11/15. </remarks>
///
/// <param name="usualThr"> The usual thr. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_console_thr()
{
    return Debug::GetInst()->logConsoleThr;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 设置立即刷新的flush on. </summary>
///
/// <remarks> Dx, 2019/3/18. </remarks>
///
/// <param name="LogSeverity"> The log severity. </param>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_set_flush_on(int LogSeverity)
{
    if (LogSeverity == DLOG_Debug)
        Debug::GetInst()->setFlushOn(spdlog::level::level_enum::debug);
    else if (LogSeverity == DLOG_INFO)
        Debug::GetInst()->setFlushOn(spdlog::level::level_enum::info);
    else if (LogSeverity == DLOG_WARNING)
        Debug::GetInst()->setFlushOn(spdlog::level::level_enum::warn);
    else if (LogSeverity == DLOG_ERROR)
        Debug::GetInst()->setFlushOn(spdlog::level::level_enum::err);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an info. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogI(const char* strFormat, ...)
{
    if (!Debug::GetInst()->isInit) { //如果还没有初始化过，那么就调用默认构造
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::GetInst()->LogI_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a w. </summary>
///
/// <remarks> Dx, 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogW(const char* strFormat, ...)
{
    if (!Debug::GetInst()->isInit) { //如果还没有初始化过，那么就调用默认构造
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::GetInst()->LogW_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an e. </summary>
///
/// <remarks> Dx, 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogE(const char* strFormat, ...)
{
    if (!Debug::GetInst()->isInit) { //如果还没有初始化过，那么就调用默认构造
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::GetInst()->LogE_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a debug. </summary>
///
/// <remarks> Dx                                                                                                                                                                                                                     , 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogD(const char* strFormat, ...)
{
    if (!Debug::GetInst()->isInit) { //如果还没有初始化过，那么就调用默认构造
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::GetInst()->LogD_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

#pragma region memory log

///-------------------------------------------------------------------------------------------------
/// <summary> 设置内存log是否使能. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="enable"> 设置为false之后内存日志函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_memory_log_enable(bool enable)
{
    Debug::GetInst()->isMemLogEnable = enable;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 提取一条内存日志,注意是一条. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="buff">   [in,out] 缓存buffer. </param>
/// <param name="offset"> The offset. </param>
/// <param name="count">  缓存buffer的长度，如果给的buffer过小那么就没办法完整提取. </param>
///
/// <returns> 提取出的日志char*的有效长度，如果为0表示没有提出日志来. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_memlog(char* buff, int offset, int length)
{
    std::string msg;
    int copyLen = 0;
    if (MemoryLog::GetInst()->getLog(msg)) {
        copyLen = msg.size() < (length - 1) ? msg.size() : (length - 1); //这里string的size是否就等于字节的size???
        msg.copy(buff + offset, copyLen);
    }
    else {
        return 0;
    }
    buff[copyLen] = '\0';
    return copyLen;
}

#pragma endregion

#pragma region 其他辅助功能

///-------------------------------------------------------------------------------------------------
/// <summary> 得到appdata的路径,目录末尾不带斜杠"C:\\Users\\dx\\AppData\\Roaming". </summary>
///
/// <remarks> Dx, 2019/3/19. </remarks>
///
/// <param name="buff">   [in] 拷贝字符的buff. </param>
/// <param name="length"> buff大小. </param>
///
/// <returns> 实际的字符串长度. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_appdata_dir(char* buff, int size)
{
    std::string dir = FileHelper::getAppDir();
    int copyLen = dir.size() < (size - 1) ? dir.size() : (size - 1); //这里string的size是否就等于字节的size???
    dir.copy(buff, copyLen);
    buff[copyLen] = '\0';
    return copyLen;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到模块的路径,目录末尾不带斜杠. </summary>
///
/// <remarks> Dx, 2019/3/19. </remarks>
///
/// <param name="buff">   [in] 拷贝字符的buff. </param>
/// <param name="length"> buff大小. </param>
///
/// <returns> 实际的字符串长度. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_module_dir(char* buff, int size)
{
    std::string dir = FileHelper::getModuleDir();
    int copyLen = dir.size() < (size - 1) ? dir.size() : (size - 1); //这里string的size是否就等于字节的size???
    dir.copy(buff, copyLen);
    buff[copyLen] = '\0';
    return copyLen;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到日志文件夹的路径. </summary>
///
/// <remarks> Dx, 2019/3/19. </remarks>
///
/// <param name="buff">   [in] 拷贝字符的buff. </param>
/// <param name="length"> buff大小. </param>
///
/// <returns> 实际的字符串长度. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_log_dir(char* buff, int size)
{
    std::string dir = Debug::GetInst()->logDirPath;
    int copyLen = dir.size() < (size - 1) ? dir.size() : (size - 1); //这里string的size是否就等于字节的size???
    dir.copy(buff, copyLen);
    buff[copyLen] = '\0';
    return copyLen;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到日志文件的路径. </summary>
///
/// <remarks> Dx, 2019/3/19. </remarks>
///
/// <param name="buff">   [in] 拷贝字符的buff. </param>
/// <param name="length"> buff大小. </param>
///
/// <returns> 实际的字符串长度. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_get_log_file_path(char* buff, int size)
{
    std::string dir = Debug::GetInst()->logFilePath;
    int copyLen = dir.size() < (size - 1) ? dir.size() : (size - 1); //这里string的size是否就等于字节的size???
    dir.copy(buff, copyLen);
    buff[copyLen] = '\0';
    return copyLen;
}
#pragma endregion
