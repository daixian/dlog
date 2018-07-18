#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string>
#include <deque>
#include <sstream>
#include "glog/logging.h"

namespace dxlib
{
    ///-------------------------------------------------------------------------------------------------
    /// <summary> 日志信息. </summary>
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
        Debug() :isLog(true), isLogI(true), isLogMemory(false)
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
            if (m_pInstance == NULL)  //判断是否第一次调用
                m_pInstance = new Debug();
            return m_pInstance;
        }

        /// <summary> 是否进行日志的记录. </summary>
        bool isLog;

        /// <summary> 低优先级的日志是否记录. </summary>
        bool isLogI;

        /// <summary> 内存日志是否记录. </summary>
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