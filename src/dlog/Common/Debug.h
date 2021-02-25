#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string>
#include <deque>
#include <sstream>
#include <mutex>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#pragma execution_character_set("utf-8")
#include "MemoryLog.h"
//#include "stack_allocator.hpp"

//栈分配器的使用(性能提升没感觉,可能也不是瓶颈,所以就不使用了)
//char buffer[DEBUG_LOG_BUFF_SIZE];
//typedef stack_allocator<char, DEBUG_LOG_BUFF_SIZE> allocator_type;
//std::vector<char, allocator_type> buf((allocator_type(buffer)));
//buf.reserve(DEBUG_LOG_BUFF_SIZE);

//一条日志的默认预估长度
#define DEBUG_LOG_BUFF_SIZE 128

namespace dlog {

/// <summary> 初始化的相对位置 </summary>
enum class INIT_RELATIVE
{
    APPDATA = 0,
    MODULE = 1,
};

///-------------------------------------------------------------------------------------------------
/// <summary> Debug日志系统. </summary>
///
/// <remarks> Dx, 2016/11/24. </remarks>
///-------------------------------------------------------------------------------------------------
class Debug
{
  private:
    /// <summary> The debug object. </summary>
    static Debug* m_pInstance;

    /// <summary> 互斥锁，用于初始化和关闭的时候. </summary>
    std::mutex mt;

    /// <summary> 默认的输出日志格式. </summary>
    const char* _logPattern = "%^[%L][%C%m%d %H:%M:%S.%e %t] %v%$";

  public:
    ///-------------------------------------------------------------------------------------------------
    /// <summary> 默认构造,默认使能LogI,不使能MemoryLog. </summary>
    ///
    /// <remarks> Dx, 2017/3/11. </remarks>
    ///-------------------------------------------------------------------------------------------------
    Debug()
    {
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Destructor. </summary>
    ///
    /// <remarks> Dx, 2017/3/11. </remarks>
    ///-------------------------------------------------------------------------------------------------
    ~Debug()
    {
        clear();
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 获得单例. </summary>
    ///
    /// <remarks> Dx, 2017/3/11. </remarks>
    ///
    /// <returns>
    /// 返回单例对象，第一次调用会创建.
    /// </returns>
    ///-------------------------------------------------------------------------------------------------
    static Debug* GetInst()
    {
        return m_pInstance;
    }

    /// <summary> 日志文件夹的记录(绝对路径). </summary>
    std::string logDirPath;

    /// <summary> 日志程序名的记录(glog中没有api来记录，所以要自己记录). </summary>
    std::string programName;

    /// <summary> 日志文件的路径. </summary>
    std::string logFilePath;

    /// <summary> 默认日志格式. https://github.com/gabime/spdlog/wiki/3.-Custom-formatting </summary>
    std::string logPattern = _logPattern;

    /// <summary> 是否使用UTF8的Bom. </summary>
    bool isUTF8BOM = true;

    /// <summary> 是否进行整个日志系统的工作. </summary>
    bool isEnable = true;

    /// <summary> 是否内存日志系统的使能. </summary>
    bool isMemLogEnable = false;

    /// <summary> 是否使能控制台. </summary>
    bool isConsoleEnable = true;

    /// <summary> 大于等于这个优先级的常规日志(文件日志)都会工作. </summary>
    spdlog::level::level_enum logFileThr = spdlog::level::level_enum::debug;

    /// <summary> 大于等于这个级别的日志都会进入内存日志. </summary>
    spdlog::level::level_enum logMemoryThr = spdlog::level::level_enum::debug;

    /// <summary> 大于等于这个级别的日志都会输出到控制台. </summary>
    spdlog::level::level_enum logConsoleThr = spdlog::level::level_enum::debug;

    /// <summary> 文件日志器. </summary>
    std::shared_ptr<spdlog::logger> filelogger = nullptr;

    /// <summary> 控制台日志器. </summary>
    std::shared_ptr<spdlog::logger> consolelogger = nullptr;

    /// <summary> 是否初始化了. </summary>
    bool isInit = false;

    /// <summary> 是否初始化失败了. </summary>
    bool isInitFail = false;

    // /// <summary> 是否使用wchat来记录日志,其实只影响内部输出的日志内容. </summary>
    // bool isWchat = false;

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 输入一个文件夹来初始化. </summary>
    ///
    /// <remarks> Dx, 2019/3/17. </remarks>
    ///
    /// <param name="logDir">  (Optional) 日志文件目录. </param>
    /// <param name="program"> (Optional) 日志程序名. </param>
    /// <param name="rel">     (Optional) 如果是相对目录那么相对位置是. </param>
    ///-------------------------------------------------------------------------------------------------
    void init(const char* logDir = "log", const char* program = "dlog",
              INIT_RELATIVE rel = INIT_RELATIVE::APPDATA, bool utf8bom = true);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 重置设置回默认设置,并且会关掉所有的日志器. </summary>
    ///
    /// <remarks> Dx, 2019/3/17. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void clear();

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 移除老的日志文件(3天前). </summary>
    ///
    /// <remarks> Surface, 2019/3/17. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void removeOldFile(long long sec = 3600 * 24 * 3);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 设置是否使能控制台日志. </summary>
    ///
    /// <remarks> Dx, 2019/3/17. </remarks>
    ///
    /// <param name="enable"> True to enable, false to
    ///                       disable. </param>
    ///-------------------------------------------------------------------------------------------------
    void setIsConsoleEnable(bool enable);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 设置立即Flush的级别. </summary>
    ///
    /// <remarks> Dx, 2019/3/18. </remarks>
    ///
    /// <param name="log_level"> The log level. </param>
    ///-------------------------------------------------------------------------------------------------
    void setFlushOn(spdlog::level::level_enum log_level)
    {
        if (filelogger != nullptr)
            filelogger->flush_on(log_level);
    }

#pragma region LOGX

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 判断是否需要输出日志,如果不需要那么就可以不进行字符格式化. </summary>
    ///
    /// <remarks> Dx, 2019/4/16. </remarks>
    ///
    /// <param name="logThr"> The log thr. </param>
    ///
    /// <returns> 如果需要才返回true. </returns>
    ///-------------------------------------------------------------------------------------------------
    bool isNeedLog(spdlog::level::level_enum logThr)
    {
        //这里由我自己来做判断，避免字符串格式化，但是实际上spdlog也是支持这个屏蔽功能的
        if (!isEnable) {
            return false; //如果控制是不输出日志
        }

        //if (!isInit && !isInitFail) {
        //    init(); //如果还没有初始化那么就初始化一次
        //}

        if (logFileThr > logThr &&
            (!isConsoleEnable || logConsoleThr > logThr) &&
            (!isMemLogEnable || logMemoryThr > logThr)) {
            return false;
        }
        return true;
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 直接输出一个完整日志. </summary>
    ///
    /// <remarks> Dx, 2019/4/16. </remarks>
    ///
    /// <param name="logThr"> The log thr. </param>
    /// <param name="msg">    The message. </param>
    ///-------------------------------------------------------------------------------------------------
    void LogMsg(spdlog::level::level_enum logThr, const char* msg)
    {
        if (logFileThr <= logThr && filelogger != nullptr) { //满足优先级才输出 - 文件
            filelogger->log(logThr, msg);
        }
        if (isConsoleEnable && logConsoleThr <= logThr && consolelogger != nullptr) { //满足优先级才输出 - 控制台
            consolelogger->log(logThr, msg);
        }
        if (isMemLogEnable && logMemoryThr <= logThr) { //满足优先级才输出 - 内存队列
            MemoryLog::GetInst()->addLog(msg);
        }
    }

#pragma endregion

    // 只有在windows上支持这个功能，暂时还是不要使用了
    //#ifdef SPDLOG_WCHAR_TO_UTF8_SUPPORT
    //    ///-------------------------------------------------------------------------------------------------
    //    /// <summary> 直接输出一个完整日志. </summary>
    //    ///
    //    /// <remarks> Dx, 2019/4/16. </remarks>
    //    ///
    //    /// <param name="logThr"> The log thr. </param>
    //    /// <param name="msg">    The message. </param>
    //    ///-------------------------------------------------------------------------------------------------
    //    void LogMsg(spdlog::level::level_enum logThr, const wchar_t* msg)
    //    {
    //        if (logFileThr <= logThr && filelogger != nullptr) { //满足优先级才输出 - 文件
    //            filelogger->log(logThr, msg);
    //        }
    //        if (isConsoleEnable && logConsoleThr <= logThr && consolelogger != nullptr) { //满足优先级才输出 - 控制台
    //            consolelogger->log(logThr, msg);
    //        }
    //        if (isMemLogEnable && logMemoryThr <= logThr) { //满足优先级才输出 - 内存队列
    //            MemoryLog::GetInst()->addLog(msg);
    //        }
    //    }
    //#endif

    // 没使用的

    //void Log(spdlog::level::level_enum logThr, const char* strFormat, ...)
    //{
    //    if (!isInit && !isInitFail) {
    //        init(); //如果还没有初始化那么就初始化一次
    //    }
    //    if (!isEnable) {
    //        return; //如果控制是不输出日志
    //    }
    //    if (logFileThr > logThr &&
    //        (!isConsoleEnable || logConsoleThr > logThr) &&
    //        (!isMemLogEnable || logMemoryThr > logThr)) {
    //        return; //如果控制是不输出DEBUG级别日志
    //    }
    //    if (NULL == strFormat) {
    //        return; //如果输入参数为空
    //    }

    //    va_list arg_ptr;
    //    va_start(arg_ptr, strFormat);

    //    std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
    //    int ret;
    //    //vsnprintf的返回是不包含\0的预留位置的
    //    while ((ret = vsnprintf(buf.data(), buf.size(), strFormat, arg_ptr)) >= buf.size()) {
    //        buf.resize(ret + 1, '\0');
    //    }
    //    va_end(arg_ptr);

    //    if (logFileThr <= logThr) { //满足优先级才输出 - 文件
    //        filelogger->log(logThr, buf.data());
    //    }
    //    if (isConsoleEnable && logConsoleThr <= logThr) { //满足优先级才输出 - 控制台
    //        consolelogger->log(logThr, buf.data());
    //    }
    //    if (isMemLogEnable && logMemoryThr <= logThr) { //满足优先级才输出 - 内存队列
    //        MemoryLog::GetInst()->addLog(buf.data());
    //    }
    //}

    //void Log_va(spdlog::level::level_enum logThr, const char* strFormat, va_list& arg_ptr)
    //{
    //    if (!isInit && !isInitFail) {
    //        init(); //如果还没有初始化那么就初始化一次
    //    }
    //    if (!isEnable) {
    //        return; //如果控制是不输出日志
    //    }
    //    if (logFileThr > logThr &&
    //        (!isConsoleEnable || logConsoleThr > logThr) &&
    //        (!isMemLogEnable || logMemoryThr > logThr)) {
    //        return; //如果控制是不输出DEBUG级别日志
    //    }
    //    if (NULL == strFormat) {
    //        return; //如果输入参数为空
    //    }

    //    std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
    //    int ret;
    //    //vsnprintf的返回是不包含\0的预留位置的
    //    while ((ret = vsnprintf(buf.data(), buf.size(), strFormat, arg_ptr)) >= buf.size()) {
    //        buf.resize(ret + 1, '\0');
    //    }
    //    if (logFileThr <= logThr) { //满足优先级才输出 - 文件
    //        filelogger->log(logThr, buf.data());
    //    }
    //    if (isConsoleEnable && logConsoleThr <= logThr) { //满足优先级才输出 - 控制台
    //        consolelogger->log(logThr, buf.data());
    //    }
    //    if (isMemLogEnable && logMemoryThr <= logThr) { //满足优先级才输出 - 内存队列
    //        MemoryLog::GetInst()->addLog(buf.data());
    //    }
    //}

    //void LogD(const char* strFormat, ...)
    //{
    //    va_list arg_ptr;
    //    va_start(arg_ptr, strFormat);
    //    Log_va(spdlog::level::level_enum::debug, strFormat, arg_ptr);
    //    va_end(arg_ptr);
    //}

    ////static void LogD(const wchar_t * strFormat, ...);

    //void LogI(const char* strFormat, ...)
    //{
    //    va_list arg_ptr;
    //    va_start(arg_ptr, strFormat);
    //    Log_va(spdlog::level::level_enum::info, strFormat, arg_ptr);
    //    va_end(arg_ptr);
    //}

    //void LogW(const char* strFormat, ...)
    //{
    //    va_list arg_ptr;
    //    va_start(arg_ptr, strFormat);
    //    Log_va(spdlog::level::level_enum::warn, strFormat, arg_ptr);
    //    va_end(arg_ptr);
    //}

    //void LogE(const char* strFormat, ...)
    //{
    //    va_list arg_ptr;
    //    va_start(arg_ptr, strFormat);
    //    Log_va(spdlog::level::level_enum::err, strFormat, arg_ptr);
    //    va_end(arg_ptr);
    //}

    //void LogC(const char* strFormat, ...)
    //{
    //    va_list arg_ptr;
    //    va_start(arg_ptr, strFormat);
    //    Log_va(spdlog::level::level_enum::critical, strFormat, arg_ptr);
    //    va_end(arg_ptr);
    //}

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 直接执行flush. </summary>
    ///
    /// <remarks> Dx, 2019/3/25. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void flush()
    {
        if (filelogger != nullptr) {
            filelogger->flush();
        }
        if (consolelogger != nullptr) {
            consolelogger->flush();
        }
    }
};

} // namespace dlog