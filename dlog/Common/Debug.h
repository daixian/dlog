#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string>
#include <deque>
#include <sstream>
#include "glog/logging.h"

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

  public:
    ///-------------------------------------------------------------------------------------------------
    /// <summary> 默认构造,默认使能LogI,不使能MemoryLog. </summary>
    ///
    /// <remarks> Dx, 2017/3/11. </remarks>
    ///-------------------------------------------------------------------------------------------------
    Debug() : isEnable(true), isMemLogEnable(false), logUsualThr(0), logMemoryThr(0)
    {
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> Destructor. </summary>
    ///
    /// <remarks> Dx, 2017/3/11. </remarks>
    ///-------------------------------------------------------------------------------------------------
    ~Debug()
    {
        Reset();
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

    /// <summary> 是否进行整个日志系统的工作. </summary>
    bool isEnable;

    /// <summary> 是否内存日志系统的使能. </summary>
    bool isMemLogEnable;

    /// <summary> 大于等于这个优先级的常规日志都会工作. </summary>
    int logUsualThr;

    /// <summary> 大于等于这个级别的日志都会进入内存日志. </summary>
    int logMemoryThr;

    /// <summary> 重置设置回默认设置. </summary>
    inline void Reset();

    inline static void LogD(const char* strFormat, ...);
    inline static void LogD_va(const char* strFormat, va_list& arg_ptr);
    //static void LogDEBUG(const wchar_t * strFormat, ...);

    inline static void LogI(const char* strFormat, ...);
    inline static void LogI_va(const char* strFormat, va_list& arg_ptr);
    //static void LogI(const wchar_t * strFormat, ...);

    inline static void LogW(const char* strFormat, ...);
    inline static void LogW_va(const char* strFormat, va_list& arg_ptr);
    //static void LogW(const wchar_t * strFormat, ...);

    inline static void LogE(const char* strFormat, ...);
    inline static void LogE_va(const char* strFormat, va_list& arg_ptr);
    //static void LogE(const wchar_t * strFormat, ...);
};

/// <summary> 重置设置. </summary>
inline void Debug::Reset()
{
    isEnable = true;
    isMemLogEnable = false;
    logUsualThr = 0;
    logMemoryThr = 0;
    MemoryLog::GetInst()->clear();
}

#pragma region LogI

inline void Debug::LogI(const char* strFormat, ...)
{
    if (m_pInstance == NULL) {
        Debug::GetInst(); //如果单例对象还未构造那么就先构造一次
    }
    if (!m_pInstance->isEnable) {
        return; //如果控制是不输出日志
    }
    if (m_pInstance->logUsualThr > LOG_THR_INFO && m_pInstance->isMemLogEnable == false ||
        m_pInstance->logUsualThr > LOG_THR_INFO && m_pInstance->logMemoryThr > LOG_THR_INFO) {
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
    if (m_pInstance->logUsualThr <= LOG_THR_INFO) { //满足优先级才输出
        LOG(INFO) << &buf[0];
    }
    va_end(arg_ptr);

    if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_INFO) { //满足优先级才输出
        MemoryLog::GetInst()->addLog(&buf[0]);
    }
}

inline void Debug::LogI_va(const char* strFormat, va_list& arg_ptr)
{
    if (m_pInstance == NULL) {
        Debug::GetInst(); //如果单例对象还未构造那么就先构造一次
    }
    if (m_pInstance->logUsualThr > LOG_THR_INFO && m_pInstance->isMemLogEnable == false ||
        m_pInstance->logUsualThr > LOG_THR_INFO && m_pInstance->logMemoryThr > LOG_THR_INFO) {
        return; //如果控制是不输出info级别日志
    }
    if (!m_pInstance->isEnable) {
        return; //如果控制是不输出日志
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
    if (m_pInstance->logUsualThr <= LOG_THR_INFO) { //满足优先级才输出
        LOG(INFO) << &buf[0];
    }

    if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_INFO) { //满足优先级才输出
        MemoryLog::GetInst()->addLog(&buf[0]);
    }
}

#pragma endregion

#pragma region LogW

inline void Debug::LogW(const char* strFormat, ...)
{
    if (m_pInstance == NULL) {
        Debug::GetInst(); //如果单例对象还未构造那么就先构造一次
    }
    if (!m_pInstance->isEnable) {
        return; //如果控制是不输出日志
    }
    if (m_pInstance->logUsualThr > LOG_THR_WARNING && m_pInstance->isMemLogEnable == false ||
        m_pInstance->logUsualThr > LOG_THR_WARNING && m_pInstance->logMemoryThr > LOG_THR_WARNING) {
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
    if (m_pInstance->logUsualThr <= LOG_THR_WARNING) { //满足优先级才输出
        LOG(WARNING) << &buf[0];
    }
    va_end(arg_ptr);

    if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_WARNING) {
        MemoryLog::GetInst()->addLog(&buf[0]);
    }
}

inline void Debug::LogW_va(const char* strFormat, va_list& arg_ptr)
{
    if (m_pInstance == NULL) {
        Debug::GetInst(); //如果单例对象还未构造那么就先构造一次
    }
    if (!m_pInstance->isEnable) {
        return; //如果控制是不输出日志
    }
    if (m_pInstance->logUsualThr > LOG_THR_WARNING && m_pInstance->isMemLogEnable == false ||
        m_pInstance->logUsualThr > LOG_THR_WARNING && m_pInstance->logMemoryThr > LOG_THR_WARNING) {
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

    if (m_pInstance->logUsualThr <= LOG_THR_WARNING) { //满足优先级才输出
        LOG(WARNING) << &buf[0];
    }

    if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_WARNING) {
        MemoryLog::GetInst()->addLog(&buf[0]);
    }
}

#pragma endregion

#pragma region LogE

inline void Debug::LogE(const char* strFormat, ...)
{
    if (m_pInstance == NULL) {
        Debug::GetInst(); //如果单例对象还未构造那么就先构造一次
    }
    if (!m_pInstance->isEnable) {
        return; //如果控制是不输出日志
    }
    if (m_pInstance->logUsualThr > LOG_THR_ERROR && m_pInstance->isMemLogEnable == false ||
        m_pInstance->logUsualThr > LOG_THR_ERROR && m_pInstance->logMemoryThr > LOG_THR_ERROR) {
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
    if (m_pInstance->logUsualThr <= LOG_THR_ERROR) { //满足优先级才输出
        LOG(ERROR) << &buf[0];
    }

    va_end(arg_ptr);

    if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_ERROR) {
        MemoryLog::GetInst()->addLog(&buf[0]);
    }
}

inline void Debug::LogE_va(const char* strFormat, va_list& arg_ptr)
{
    if (m_pInstance == NULL) {
        Debug::GetInst(); //如果单例对象还未构造那么就先构造一次
    }
    if (!m_pInstance->isEnable) {
        return; //如果控制是不输出日志
    }
    if (m_pInstance->logUsualThr > LOG_THR_ERROR && m_pInstance->isMemLogEnable == false ||
        m_pInstance->logUsualThr > LOG_THR_ERROR && m_pInstance->logMemoryThr > LOG_THR_ERROR) {
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
    if (m_pInstance->logUsualThr <= LOG_THR_ERROR) { //满足优先级才输出
        LOG(ERROR) << &buf[0];
    }

    if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_ERROR) {
        MemoryLog::GetInst()->addLog(&buf[0]);
    }
}

#pragma endregion

#pragma region LogD

inline void Debug::LogD(const char* strFormat, ...)
{
    if (m_pInstance == NULL) {
        Debug::GetInst(); //如果单例对象还未构造那么就先构造一次
    }
    if (!m_pInstance->isEnable) {
        return; //如果控制是不输出日志
    }
    if (m_pInstance->logUsualThr > LOG_THR_DEBUG && m_pInstance->isMemLogEnable == false ||
        m_pInstance->logUsualThr > LOG_THR_DEBUG && m_pInstance->logMemoryThr > LOG_THR_DEBUG) {
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
    if (m_pInstance->logUsualThr <= LOG_THR_DEBUG) { //满足优先级才输出
        LOG(INFO) << &buf[0];
    }

    va_end(arg_ptr);

    if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_DEBUG) {
        MemoryLog::GetInst()->addLog(&buf[0]);
    }
}

inline void Debug::LogD_va(const char* strFormat, va_list& arg_ptr)
{
    if (m_pInstance == NULL) {
        Debug::GetInst(); //如果单例对象还未构造那么就先构造一次
    }
    if (!m_pInstance->isEnable) {
        return; //如果控制是不输出日志
    }
    if (m_pInstance->logUsualThr > LOG_THR_DEBUG && m_pInstance->isMemLogEnable == false ||
        m_pInstance->logUsualThr > LOG_THR_DEBUG && m_pInstance->logMemoryThr > LOG_THR_DEBUG) {
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
    if (m_pInstance->logUsualThr <= LOG_THR_DEBUG) { //满足优先级才输出
        LOG(INFO) << &buf[0];
    }

    if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_DEBUG) {
        MemoryLog::GetInst()->addLog(&buf[0]);
    }
}

#pragma endregion

} // namespace dxlib