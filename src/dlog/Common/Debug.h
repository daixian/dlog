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

// 初始化的相对位置
enum class INIT_RELATIVE
{
    APPDATA = 0,
    MODULE = 1,
};

// 日志回调
typedef void (*LoggerCallback)(int level, const char* message);

/**
 * Debug日志系统.
 *
 * @author daixian
 * @date 2016/11/24
 */
class Debug
{
  private:
    // 单例
    static Debug* m_pInstance;

    // 互斥锁，用于初始化和关闭的时候.
    std::mutex mt;

    // 默认的输出日志格式.
    const char* _logPattern = "%^[%L][%C%m%d %H:%M:%S.%e %t] %v%$";

  public:
    /**
     * 默认构造,默认使能LogI,不使能MemoryLog.
     *
     * @author daixian
     * @date 2017/3/11
     */
    Debug()
    {
    }

    /**
     * Destructor.
     *
     * @author daixian
     * @date 2017/3/11
     */
    ~Debug()
    {
        clear();
    }

    /**
     * 获得单例.
     *
     * @author daixian
     * @date 2017/3/11
     *
     * @returns 返回单例对象，第一次调用会创建.
     */
    static Debug* GetInst()
    {
        return m_pInstance;
    }

    // 日志文件夹的记录(绝对路径).
    std::string logDirPath;

    // 日志程序名的记录(glog中没有api来记录，所以要自己记录).
    std::string programName;

    // 日志文件的路径.
    std::string logFilePath;

    // 默认日志格式. https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    std::string logPattern = _logPattern;

    // 是否使用UTF8的Bom.
    bool isUTF8BOM = true;

    // 是否进行整个日志系统的工作.
    bool isEnable = true;

    // 是否内存日志系统的使能.
    bool isMemLogEnable = false;

    // 是否使能控制台.
    bool isConsoleEnable = true;

    // 大于等于这个优先级的常规日志(文件日志)都会工作.
    spdlog::level::level_enum logFileThr = spdlog::level::level_enum::debug;

    // 大于等于这个级别的日志都会进入内存日志.
    spdlog::level::level_enum logMemoryThr = spdlog::level::level_enum::debug;

    // 大于等于这个级别的日志都会输出到控制台.
    spdlog::level::level_enum logConsoleThr = spdlog::level::level_enum::debug;

    // 文件日志器.
    std::shared_ptr<spdlog::logger> filelogger = nullptr;

    // 控制台日志器.
    std::shared_ptr<spdlog::logger> consolelogger = nullptr;

    // 是否初始化了.
    bool isInit = false;

    // 是否初始化失败了.
    bool isInitFail = false;

    // 外部传入的logger指针
    LoggerCallback exLoggerCallback = nullptr;

    // 是否执行chcp 65001
    bool ischcp65001 = false;

    // 是否打印模块内部的消息
    bool isPrintInternalMessage = false;

    // 是否使用wchat来记录日志,其实只影响内部输出的日志内容.
    // bool isWchat = false;

    /**
     * 输入一个文件夹来初始化.
     *
     * @author daixian
     * @date 2019/3/17
     *
     * @param  logDir  (Optional) 日志文件目录.
     * @param  program (Optional) 日志程序名.
     * @param  rel     (Optional) 如果是相对目录那么相对位置是.
     * @param  utf8bom (Optional) True to UTF 8bom.
     */
    void init(const char* logDir = "log", const char* program = "dlog",
              INIT_RELATIVE rel = INIT_RELATIVE::APPDATA, bool utf8bom = true);

    /**
     * 重置设置回默认设置,并且会关掉所有的日志器.
     *
     * @author daixian
     * @date 2019/3/17
     */
    void clear();

    /**
     * 移除老的日志文件(3天前).
     *
     * @author daixian
     * @date 2019/3/17
     *
     * @param  sec (Optional) 秒数.
     */
    void removeOldFile(long long sec = 3600 * 24 * 3);

    /**
     * 设置是否使能控制台日志.
     *
     * @author daixian
     * @date 2019/3/17
     *
     * @param  enable True to enable, false to disable.
     */
    void setIsConsoleEnable(bool enable);

    /**
     * 设置立即Flush的级别.
     *
     * @author daixian
     * @date 2019/3/18
     *
     * @param  log_level The log level.
     */
    void setFlushOn(spdlog::level::level_enum log_level)
    {
        if (filelogger != nullptr)
            filelogger->flush_on(log_level);
    }

    /**
     * 判断是否需要输出日志,如果不需要那么就可以不进行字符格式化.
     *
     * @author daixian
     * @date 2019/4/16
     *
     * @param  logThr The log thr.
     *
     * @returns 如果需要才返回true.
     */
    bool isNeedLog(spdlog::level::level_enum logThr)
    {
        //这里由我自己来做判断，避免字符串格式化，但是实际上spdlog也是支持这个屏蔽功能的
        if (!isEnable) {
            return false; //如果控制是不输出日志
        }
        if (exLoggerCallback != nullptr) {
            return true;
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

    /**
     * 直接输出一个完整日志.
     *
     * @author daixian
     * @date 2019/4/16
     *
     * @param  logThr The log thr.
     * @param  msg    The message.
     */
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

        //如果有外部的回调那么就执行一下
        if (exLoggerCallback != nullptr) {
            exLoggerCallback((int)logThr, msg);
        }
    }

    /**
     * 直接执行flush.
     *
     * @author daixian
     * @date 2019/3/25
     */
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