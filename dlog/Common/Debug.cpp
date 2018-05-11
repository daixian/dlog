#pragma once
#include "Debug.h"
#include "MemoryLog.h"

//һ����־�ĳ���
#define DEBUG_LOG_BUFF_SIZE 128

namespace dxlib
{
    Debug* Debug::m_pInstance = NULL;

    void Debug::LogI(const char * strFormat, ...)
    {
        if (m_pInstance == NULL) { Debug::GetInst(); }//�����������δ������ô���ȹ���һ��
        if (!m_pInstance->isLogI) { return; }	//��������ǲ���������ȼ���־)
        if (!m_pInstance->isLog) { return; }	//��������ǲ������־
        if (NULL == strFormat) { return; }//����������Ϊ��

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
        if (m_pInstance == NULL) { Debug::GetInst(); }//�����������δ������ô���ȹ���һ��
        if (!m_pInstance->isLog) { return; }	//��������ǲ������־
        if (NULL == strFormat) { return; }//����������Ϊ��

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
        if (m_pInstance == NULL) { Debug::GetInst(); }//�����������δ������ô���ȹ���һ��
        if (!m_pInstance->isLog) { return; }	//��������ǲ������־
        if (NULL == strFormat) { return; }//����������Ϊ��

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
        if (m_pInstance == NULL) { Debug::GetInst(); }//�����������δ������ô���ȹ���һ��
        if (!m_pInstance->isLog) { return; }	//��������ǲ������־
        if (NULL == strFormat) { return; }//����������Ϊ��

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
        if (m_pInstance == NULL) { Debug::GetInst(); }//�����������δ������ô���ȹ���һ��
        if (!m_pInstance->isLogI) { return; }	//��������ǲ���������ȼ���־)
        if (!m_pInstance->isLog) { return; }	//��������ǲ������־
        if (NULL == strFormat) { return; }//����������Ϊ��

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
        if (m_pInstance == NULL) { Debug::GetInst(); }//�����������δ������ô���ȹ���һ��
        if (!m_pInstance->isLog) { return; }	//��������ǲ������־
        if (NULL == strFormat) { return; }//����������Ϊ��

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
        if (m_pInstance == NULL) { Debug::GetInst(); }//�����������δ������ô���ȹ���һ��
        if (!m_pInstance->isLog) { return; }	//��������ǲ������־
        if (NULL == strFormat) { return; }//����������Ϊ��

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
        if (m_pInstance == NULL) { Debug::GetInst(); }//�����������δ������ô���ȹ���һ��
        if (!m_pInstance->isLog) { return; }	//��������ǲ������־
        if (NULL == strFormat) { return; }//����������Ϊ��

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