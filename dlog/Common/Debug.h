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

#include "MemoryLog.h"
//#include "stack_allocator.hpp"

//栈分配器的使用(性能提升没感觉,可能也不是瓶颈,所以就不使用了)
//char buffer[DEBUG_LOG_BUFF_SIZE];
//typedef stack_allocator<char, DEBUG_LOG_BUFF_SIZE> allocator_type;
//std::vector<char, allocator_type> buf((allocator_type(buffer)));
//buf.reserve(DEBUG_LOG_BUFF_SIZE);

//一条日志的默认预估长度
#define DEBUG_LOG_BUFF_SIZE 128

namespace dxlib {

//几种优先级的定义,由于构造的时候门限被设为了0,所以默认没有使能DEBUG
#define LOG_THR_DEBUG -1
#define LOG_THR_INFO 0
#define LOG_THR_WARNING 1
#define LOG_THR_ERROR 2
#define LOG_THR_FATAL 3

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
        if (m_pInstance == NULL) //判断是否第一次调用
            m_pInstance = new Debug();
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

    /// <summary> 是否进行整个日志系统的工作. </summary>
    bool isEnable = true;

    /// <summary> 是否内存日志系统的使能. </summary>
    bool isMemLogEnable = false;

    /// <summary> 是否使能控制台. </summary>
    bool isConsoleEnable = true;

    /// <summary> 大于等于这个优先级的常规日志(文件日志)都会工作. </summary>
    int logUsualThr = LOG_THR_INFO;

    /// <summary> 大于等于这个级别的日志都会进入内存日志. </summary>
    int logMemoryThr = LOG_THR_INFO;

    /// <summary> 大于等于这个级别的日志都会输出到控制台. </summary>
    int logConsoleThr = LOG_THR_INFO;

    /// <summary> 文件日志器. </summary>
    std::shared_ptr<spdlog::logger> filelogger = nullptr;

    /// <summary> 控制台日志器. </summary>
    std::shared_ptr<spdlog::logger> consolelogger = nullptr;

    /// <summary> 是否初始化了. </summary>
    bool isInit = false;

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 输入一个文件夹来初始化. </summary>
    ///
    /// <remarks> Dx, 2019/3/17. </remarks>
    ///
    /// <param name="program"> The program. </param>
    /// <param name="logDir">  (Optional) The log dir. </param>
    ///-------------------------------------------------------------------------------------------------
    void init(const char* program = "dlog", const char* logDir = "log");

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 重置设置回默认设置. </summary>
    ///
    /// <remarks> Dx, 2019/3/17. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void clear();

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 移除老的日志文件(3天前). </summary>
    ///
    /// <remarks> Surface, 2019/3/17. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void removeOldFile(long sec = 3600 * 24 * 3);

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 设置是否使能控制台日志. </summary>
    ///
    /// <remarks> Dx, 2019/3/17. </remarks>
    ///
    /// <param name="enable"> True to enable, false to
    ///                       disable. </param>
    ///-------------------------------------------------------------------------------------------------
    void setIsConsoleEnable(bool enable);

#pragma region LOGX

    void LogD(const char* strFormat, ...)
    {
        if (!isInit) {
            init(); //如果还没有初始化那么就初始化一次
        }
        if (!isEnable) {
            return; //如果控制是不输出日志
        }
        if (logUsualThr > LOG_THR_DEBUG &&
            (!isConsoleEnable || logConsoleThr > LOG_THR_DEBUG) &&
            (!isMemLogEnable || logMemoryThr > LOG_THR_DEBUG)) {
            return; //如果控制是不输出DEBUG级别日志
        }
        if (NULL == strFormat) {
            return; //如果输入参数为空
        }

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (logUsualThr <= LOG_THR_DEBUG) { //满足优先级才输出
            filelogger->debug(&buf[0]);
        }
        if (isConsoleEnable && logConsoleThr <= LOG_THR_DEBUG) { //满足优先级才输出
            consolelogger->debug(&buf[0]);
        }

        va_end(arg_ptr);

        if (isMemLogEnable && logMemoryThr <= LOG_THR_DEBUG) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void LogD_va(const char* strFormat, va_list& arg_ptr)
    {
        if (!isInit) {
            init(); //如果还没有初始化那么就初始化一次
        }
        if (!isEnable) {
            return; //如果控制是不输出日志
        }
        if (logUsualThr > LOG_THR_DEBUG &&
            (!isConsoleEnable || logConsoleThr > LOG_THR_DEBUG) &&
            (!isMemLogEnable || logMemoryThr > LOG_THR_DEBUG)) {
            return; //如果控制是不输出DEBUG级别日志
        }
        if (NULL == strFormat) {
            return; //如果输入参数为空
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (logUsualThr <= LOG_THR_DEBUG) { //满足优先级才输出
            filelogger->debug(&buf[0]);
        }
        if (isConsoleEnable && logConsoleThr <= LOG_THR_DEBUG) { //满足优先级才输出
            consolelogger->debug(&buf[0]);
        }

        if (isMemLogEnable && logMemoryThr <= LOG_THR_DEBUG) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    //static void LogDEBUG(const wchar_t * strFormat, ...);

    void LogI(const char* strFormat, ...)
    {
        if (!isInit) {
            init(); //如果还没有初始化那么就初始化一次
        }
        if (!isEnable) {
            return; //如果控制是不输出日志
        }
        if (logUsualThr > LOG_THR_INFO &&
            (!isConsoleEnable || logConsoleThr > LOG_THR_INFO) &&
            (!isMemLogEnable || logMemoryThr > LOG_THR_INFO)) {
            return; //如果控制是不输出info级别日志
        }
        if (NULL == strFormat) {
            return; //如果输入参数为空
        }

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (logUsualThr <= LOG_THR_INFO) { //满足优先级才输出
            filelogger->info(&buf[0]);
        }
        if (isConsoleEnable && logConsoleThr <= LOG_THR_INFO) { //满足优先级才输出
            consolelogger->info(&buf[0]);
        }

        va_end(arg_ptr);

        if (isMemLogEnable && logMemoryThr <= LOG_THR_INFO) { //满足优先级才输出
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void LogI_va(const char* strFormat, va_list& arg_ptr)
    {
        if (!isInit) {
            init(); //如果还没有初始化那么就初始化一次
        }
        if (!isEnable) {
            return; //如果控制是不输出日志
        }
        if (logUsualThr > LOG_THR_INFO &&
            (!isConsoleEnable || logConsoleThr > LOG_THR_INFO) &&
            (!isMemLogEnable || logMemoryThr > LOG_THR_INFO)) {
            return; //如果控制是不输出info级别日志
        }
        if (NULL == strFormat) {
            return; //如果输入参数为空
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (logUsualThr <= LOG_THR_INFO) { //满足优先级才输出
            filelogger->info(&buf[0]);
        }
        if (isConsoleEnable && logConsoleThr <= LOG_THR_INFO) { //满足优先级才输出
            consolelogger->info(&buf[0]);
        }

        if (isMemLogEnable && logMemoryThr <= LOG_THR_INFO) { //满足优先级才输出
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    //static void LogI(const wchar_t * strFormat, ...);

    void LogW(const char* strFormat, ...)
    {
        if (!isInit) {
            init(); //如果还没有初始化那么就初始化一次
        }
        if (!isEnable) {
            return; //如果控制是不输出日志
        }
        if (logUsualThr > LOG_THR_WARNING &&
            (!isConsoleEnable || logConsoleThr > LOG_THR_WARNING) &&
            (!isMemLogEnable || logMemoryThr > LOG_THR_WARNING)) {
            return; //如果控制是不输出warning级别日志
        }
        if (NULL == strFormat) {
            return; //如果输入参数为空
        }

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (logUsualThr <= LOG_THR_WARNING) { //满足优先级才输出
            filelogger->warn(&buf[0]);
        }
        if (isConsoleEnable && logConsoleThr <= LOG_THR_WARNING) { //满足优先级才输出
            consolelogger->warn(&buf[0]);
        }

        va_end(arg_ptr);

        if (isMemLogEnable && logMemoryThr <= LOG_THR_WARNING) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void LogW_va(const char* strFormat, va_list& arg_ptr)
    {
        if (!isInit) {
            init(); //如果还没有初始化那么就初始化一次
        }
        if (!isEnable) {
            return; //如果控制是不输出日志
        }
        if (logUsualThr > LOG_THR_WARNING &&
            (!isConsoleEnable || logConsoleThr > LOG_THR_WARNING) &&
            (!isMemLogEnable || logMemoryThr > LOG_THR_WARNING)) {
            return; //如果控制是不输出warning级别日志
        }
        if (NULL == strFormat) {
            return; //如果输入参数为空
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';

        if (logUsualThr <= LOG_THR_WARNING) { //满足优先级才输出
            filelogger->warn(&buf[0]);
        }
        if (isConsoleEnable && logConsoleThr <= LOG_THR_WARNING) { //满足优先级才输出
            consolelogger->warn(&buf[0]);
        }

        if (isMemLogEnable && logMemoryThr <= LOG_THR_WARNING) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    //static void LogW(const wchar_t * strFormat, ...);

    void LogE(const char* strFormat, ...)
    {
        if (!isInit) {
            init(); //如果还没有初始化那么就初始化一次
        }
        if (!isEnable) {
            return; //如果控制是不输出日志
        }
        if (logUsualThr > LOG_THR_ERROR &&
            (!isConsoleEnable || logConsoleThr > LOG_THR_ERROR) &&
            (!isMemLogEnable || logMemoryThr > LOG_THR_ERROR)) {
            return; //如果控制是不输出error级别日志
        }
        if (NULL == strFormat) {
            return; //如果输入参数为空
        }

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (logUsualThr <= LOG_THR_ERROR) { //满足优先级才输出
            filelogger->error(&buf[0]);
        }
        if (isConsoleEnable && logConsoleThr <= LOG_THR_ERROR) { //满足优先级才输出
            consolelogger->error(&buf[0]);
        }

        va_end(arg_ptr);

        if (isMemLogEnable && logMemoryThr <= LOG_THR_ERROR) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void LogE_va(const char* strFormat, va_list& arg_ptr)
    {
        if (!isInit) {
            init(); //如果还没有初始化那么就初始化一次
        }
        if (!isEnable) {
            return; //如果控制是不输出日志
        }
        if (logUsualThr > LOG_THR_ERROR &&
            (!isConsoleEnable || logConsoleThr > LOG_THR_ERROR) &&
            (!isMemLogEnable || logMemoryThr > LOG_THR_ERROR)) {
            return; //如果控制是不输出error级别日志
        }
        if (NULL == strFormat) {
            return; //如果输入参数为空
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (logUsualThr <= LOG_THR_ERROR) { //满足优先级才输出
            filelogger->error(&buf[0]);
        }
        if (isConsoleEnable && logConsoleThr <= LOG_THR_ERROR) { //满足优先级才输出
            consolelogger->error(&buf[0]);
        }

        if (isMemLogEnable && logMemoryThr <= LOG_THR_ERROR) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }
    //static void LogE(const wchar_t * strFormat, ...);

#pragma endregion
};

} // namespace dxlib