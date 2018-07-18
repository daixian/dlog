#pragma once
#include "Debug.h"
#include "MemoryLog.h"

//一条日志的长度
#define DEBUG_LOG_BUFF_SIZE 128

namespace dxlib
{
    Debug* Debug::m_pInstance = NULL;

    void Debug::LogI(const char * strFormat, ...)
    {
        if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
        if (!m_pInstance->isLogI) { return; }	//如果控制是不输出低优先级日志)
        if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
        if (NULL == strFormat) { return; }//如果输入参数为空

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1)
        {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        LOG(INFO) << &buf[0];
        va_end(arg_ptr);

        if (m_pInstance->isLogMemory)
        {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogW(const char * strFormat, ...)
    {
        if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
        if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
        if (NULL == strFormat) { return; }//如果输入参数为空

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1)
        {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        LOG(WARNING) << &buf[0];
        va_end(arg_ptr);

        if (m_pInstance->isLogMemory)
        {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogE(const char * strFormat, ...)
    {
        if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
        if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
        if (NULL == strFormat) { return; }//如果输入参数为空

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1)
        {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        LOG(ERROR) << &buf[0];

        va_end(arg_ptr);

        if (m_pInstance->isLogMemory)
        {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogFATAL(const char * strFormat, ...)
    {
        if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
        if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
        if (NULL == strFormat) { return; }//如果输入参数为空

        va_list arg_ptr = NULL;
        va_start(arg_ptr, strFormat);

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1)
        {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        LOG(FATAL) << &buf[0];

        va_end(arg_ptr);

        if (m_pInstance->isLogMemory)
        {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogI_va(const char * strFormat, va_list &arg_ptr)
    {
        if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
        if (!m_pInstance->isLogI) { return; }	//如果控制是不输出低优先级日志)
        if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
        if (NULL == strFormat) { return; }//如果输入参数为空

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1)
        {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        LOG(INFO) << &buf[0];

        if (m_pInstance->isLogMemory)
        {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogW_va(const char * strFormat, va_list &arg_ptr)
    {
        if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
        if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
        if (NULL == strFormat) { return; }//如果输入参数为空

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1)
        {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        LOG(WARNING) << &buf[0];

        if (m_pInstance->isLogMemory)
        {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogE_va(const char * strFormat, va_list &arg_ptr)
    {
        if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
        if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
        if (NULL == strFormat) { return; }//如果输入参数为空

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1)
        {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        LOG(ERROR) << &buf[0];

        if (m_pInstance->isLogMemory)
        {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }

    void Debug::LogFATAL_va(const char * strFormat, va_list &arg_ptr)
    {
        if (m_pInstance == NULL) { Debug::GetInst(); }//如果单例对象还未构造那么就先构造一次
        if (!m_pInstance->isLog) { return; }	//如果控制是不输出日志
        if (NULL == strFormat) { return; }//如果输入参数为空

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret;
        while ((ret = vsnprintf_s(&buf[0], buf.size() - 1, _TRUNCATE, strFormat, arg_ptr)) == -1)
        {
            buf.resize(buf.size() * 2);
        }
        buf[ret] = '\0';
        LOG(FATAL) << &buf[0];

        if (m_pInstance->isLogMemory)
        {
            MemoryLog::GetInst()->addLog(&buf[0]);
        }
    }
}