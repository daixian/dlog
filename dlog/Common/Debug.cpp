#pragma once
#include "Debug.h"
#include "MemoryLog.h"

//一条日志的默认预估长度
#define DEBUG_LOG_BUFF_SIZE 128

namespace dxlib {

    Debug* Debug::m_pInstance = NULL;

    /// <summary> 重置设置. </summary>
    void Debug::Reset()
    {
        isEnable = true;
        isMemLogEnable = false;
        logUsualThr = 0;
        logMemoryThr = 0;  
        MemoryLog::GetInst()->clear();
    }

    #pragma region LogI

    void Debug::LogI(const char* strFormat, ...)
    {
        if (m_pInstance == NULL) {
            Debug::GetInst();    //如果单例对象还未构造那么就先构造一次
        }
        if (!m_pInstance->isEnable) {
            return;    //如果控制是不输出日志
        }
        if (m_pInstance->logUsualThr > LOG_THR_INFO && m_pInstance->isMemLogEnable == false ||
                m_pInstance->logUsualThr > LOG_THR_INFO && m_pInstance->logMemoryThr > LOG_THR_INFO) {
            return;    //如果控制是不输出info级别日志
        }
        if (NULL == strFormat) {
            return;    //如果输入参数为空
        }

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if(m_pInstance->logUsualThr <= LOG_THR_INFO) {//满足优先级才输出
            LOG(INFO) << &buf[0];
        }
        va_end(arg_ptr);

        if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_INFO) {//满足优先级才输出
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogI_va(const char* strFormat, va_list& arg_ptr)
    {
        if (m_pInstance == NULL) {
            Debug::GetInst();    //如果单例对象还未构造那么就先构造一次
        }
        if (m_pInstance->logUsualThr > LOG_THR_INFO && m_pInstance->isMemLogEnable == false ||
                m_pInstance->logUsualThr > LOG_THR_INFO && m_pInstance->logMemoryThr > LOG_THR_INFO) {
            return;    //如果控制是不输出info级别日志
        }
        if (!m_pInstance->isEnable) {
            return;    //如果控制是不输出日志
        }
        if (NULL == strFormat) {
            return;    //如果输入参数为空
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (m_pInstance->logUsualThr <= LOG_THR_INFO) {//满足优先级才输出
            LOG(INFO) << &buf[0];
        }

        if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_INFO) {//满足优先级才输出
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    #pragma endregion

    #pragma region LogW

    void Debug::LogW(const char* strFormat, ...)
    {
        if (m_pInstance == NULL) {
            Debug::GetInst();    //如果单例对象还未构造那么就先构造一次
        }
        if (!m_pInstance->isEnable) {
            return;    //如果控制是不输出日志
        }
        if (m_pInstance->logUsualThr > LOG_THR_WARNING && m_pInstance->isMemLogEnable == false ||
                m_pInstance->logUsualThr > LOG_THR_WARNING && m_pInstance->logMemoryThr > LOG_THR_WARNING) {
            return;    //如果控制是不输出warning级别日志
        }
        if (NULL == strFormat) {
            return;    //如果输入参数为空
        }

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (m_pInstance->logUsualThr <= LOG_THR_WARNING) {//满足优先级才输出
            LOG(WARNING) << &buf[0];
        }
        va_end(arg_ptr);

        if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_WARNING) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogW_va(const char* strFormat, va_list& arg_ptr)
    {
        if (m_pInstance == NULL) {
            Debug::GetInst();    //如果单例对象还未构造那么就先构造一次
        }
        if (!m_pInstance->isEnable) {
            return;    //如果控制是不输出日志
        }
        if (m_pInstance->logUsualThr > LOG_THR_WARNING && m_pInstance->isMemLogEnable == false ||
                m_pInstance->logUsualThr > LOG_THR_WARNING && m_pInstance->logMemoryThr > LOG_THR_WARNING) {
            return;    //如果控制是不输出warning级别日志
        }
        if (NULL == strFormat) {
            return;    //如果输入参数为空
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';

        if (m_pInstance->logUsualThr <= LOG_THR_WARNING) {//满足优先级才输出
            LOG(WARNING) << &buf[0];
        }

        if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_WARNING) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    #pragma endregion

    #pragma region LogE

    void Debug::LogE(const char* strFormat, ...)
    {
        if (m_pInstance == NULL) {
            Debug::GetInst();    //如果单例对象还未构造那么就先构造一次
        }
        if (!m_pInstance->isEnable) {
            return;    //如果控制是不输出日志
        }
        if (m_pInstance->logUsualThr > LOG_THR_ERROR && m_pInstance->isMemLogEnable == false ||
                m_pInstance->logUsualThr > LOG_THR_ERROR && m_pInstance->logMemoryThr > LOG_THR_ERROR) {
            return;    //如果控制是不输出error级别日志
        }
        if (NULL == strFormat) {
            return;    //如果输入参数为空
        }

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (m_pInstance->logUsualThr <= LOG_THR_ERROR) {//满足优先级才输出
            LOG(ERROR) << &buf[0];
        }

        va_end(arg_ptr);

        if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_ERROR) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogE_va(const char* strFormat, va_list& arg_ptr)
    {
        if (m_pInstance == NULL) {
            Debug::GetInst();    //如果单例对象还未构造那么就先构造一次
        }
        if (!m_pInstance->isEnable) {
            return;    //如果控制是不输出日志
        }
        if (m_pInstance->logUsualThr > LOG_THR_ERROR && m_pInstance->isMemLogEnable == false ||
                m_pInstance->logUsualThr > LOG_THR_ERROR && m_pInstance->logMemoryThr > LOG_THR_ERROR) {
            return;    //如果控制是不输出error级别日志
        }
        if (NULL == strFormat) {
            return;    //如果输入参数为空
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (m_pInstance->logUsualThr <= LOG_THR_ERROR) {//满足优先级才输出
            LOG(ERROR) << &buf[0];
        }

        if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_ERROR) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    #pragma endregion

    #pragma region LogD

    void Debug::LogD(const char* strFormat, ...)
    {
        if (m_pInstance == NULL) {
            Debug::GetInst();    //如果单例对象还未构造那么就先构造一次
        }
        if (!m_pInstance->isEnable) {
            return;    //如果控制是不输出日志
        }
        if (m_pInstance->logUsualThr > LOG_THR_DEBUG && m_pInstance->isMemLogEnable == false ||
                m_pInstance->logUsualThr > LOG_THR_DEBUG && m_pInstance->logMemoryThr > LOG_THR_DEBUG) {
            return;    //如果控制是不输出DEBUG级别日志
        }
        if (NULL == strFormat) {
            return;    //如果输入参数为空
        }

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (m_pInstance->logUsualThr <= LOG_THR_DEBUG) {//满足优先级才输出
            LOG(INFO) << &buf[0];
        }

        va_end(arg_ptr);

        if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_DEBUG) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogD_va(const char* strFormat, va_list& arg_ptr)
    {
        if (m_pInstance == NULL) {
            Debug::GetInst();    //如果单例对象还未构造那么就先构造一次
        }
        if (!m_pInstance->isEnable) {
            return;    //如果控制是不输出日志
        }
        if (m_pInstance->logUsualThr > LOG_THR_DEBUG && m_pInstance->isMemLogEnable == false ||
                m_pInstance->logUsualThr > LOG_THR_DEBUG && m_pInstance->logMemoryThr > LOG_THR_DEBUG) {
            return;    //如果控制是不输出DEBUG级别日志
        }
        if (NULL == strFormat) {
            return;    //如果输入参数为空
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1) {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        if (m_pInstance->logUsualThr <= LOG_THR_DEBUG) {//满足优先级才输出
            LOG(INFO) << &buf[0];
        }

        if (m_pInstance->isMemLogEnable && m_pInstance->logMemoryThr <= LOG_THR_DEBUG) {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }
    #pragma endregion
}