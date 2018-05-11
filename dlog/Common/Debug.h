#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string>
#include <deque>
#include <sstream>
#include "../glog/logging.h"

namespace dxlib
{
    ///-------------------------------------------------------------------------------------------------
    /// <summary> ��־��Ϣ. </summary>
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
        /// <summary> Ĭ�Ϲ���. </summary>
        ///
        /// <remarks> Dx, 2017/3/11. </remarks>
        ///-------------------------------------------------------------------------------------------------
        Debug() :isLog(true), isLogI(true)
        {
        }

        ///-------------------------------------------------------------------------------------------------
        /// <summary> Destructor. </summary>
        ///
        /// <remarks> Dx, 2017/3/11. </remarks>
        ///-------------------------------------------------------------------------------------------------
        ~Debug()
        {
            //for (auto &item : memLogPool)
            //{
            //    delete item;
            //}
        }

        ///-------------------------------------------------------------------------------------------------
        /// <summary> ��õ���. </summary>
        ///
        /// <remarks> Dx, 2017/3/11. </remarks>
        ///
        /// <returns>
        /// ���ص������󣬵�һ�ε��ûᴴ��.
        /// </returns>
        ///-------------------------------------------------------------------------------------------------
        static Debug* GetInst()
        {
            if (m_pInstance == NULL)  //�ж��Ƿ��һ�ε���
                m_pInstance = new Debug();
            return m_pInstance;
        }

        /// <summary> �Ƿ������־�ļ�¼. </summary>
        bool isLog;

        /// <summary> �����ȼ�����־�Ƿ��¼. </summary>
        bool isLogI;

        /// <summary> �ڴ���־�Ƿ��¼. </summary>
        bool isLogMemory;

        static void LogI(const char * strFormat, ...);
        static void LogI_va(const char * strFormat, va_list &arg_ptr);
        //static void LogI(const wchar_t * strFormat, ...);

        static void LogW(const char * strFormat, ...);
        static void LogW_va(const char * strFormat, va_list &arg_ptr);
        //static void LogW(const wchar_t * strFormat, ...);

        static void LogE(const char * strFormat, ...);
        static void LogE_va(const char * strFormat, va_list &arg_ptr);
        //static void LogE(const wchar_t * strFormat, ...);

        static void LogFATAL(const char * strFormat, ...);
        static void LogFATAL_va(const char * strFormat, va_list &arg_ptr);
        //static void LogFATAL(const wchar_t * strFormat, ...);

    };
}