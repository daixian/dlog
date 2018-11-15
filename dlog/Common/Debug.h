#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string>
#include <deque>
#include <sstream>
#include "glog/logging.h"

namespace dxlib {

    //几种优先级的定义
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
        Debug(): isEnable(true), isMemLogEnable(false), logUsualThr(0), logMemoryThr(0)
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
            if (m_pInstance == NULL)  //判断是否第一次调用
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
        void Reset();

        static void LogD(const char* strFormat, ...);
        static void LogD_va(const char* strFormat, va_list& arg_ptr);
        //static void LogDEBUG(const wchar_t * strFormat, ...);

        static void LogI(const char* strFormat, ...);
        static void LogI_va(const char* strFormat, va_list& arg_ptr);
        //static void LogI(const wchar_t * strFormat, ...);

        static void LogW(const char* strFormat, ...);
        static void LogW_va(const char* strFormat, va_list& arg_ptr);
        //static void LogW(const wchar_t * strFormat, ...);

        static void LogE(const char* strFormat, ...);
        static void LogE_va(const char* strFormat, va_list& arg_ptr);
        //static void LogE(const wchar_t * strFormat, ...);



    };
}