#pragma once

#include <vector>
#include <string>

#if defined DLOG_EXPORTS
#define DLOG_EXPORT __declspec(dllexport)
#else
#pragma comment(lib, "dlog.lib")
#define DLOG_EXPORT __declspec(dllimport)
#endif

//LogSeverity define
#define DLOG_INFO 0
#define DLOG_WARNING 1
#define DLOG_ERROR 2
#define DLOG_FATAL 3

namespace dlog
{
    enum LogSeverity
    {
        Log_INFO = 0,
        Log_WARNING = 1,
        Log_ERROR = 2,
        Log_FATAL = 3
    };
}
///-------------------------------------------------------------------------------------------------
/// <summary> 模块初始化. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="logDir">  [in]日志文件夹路径名（相对模块目录）. </param>
/// <param name="program"> [in]程序名. </param>
///
/// <returns> 如果之前未被初始化返回0，否则返回1. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_init(char* logDir = "\\log", char* program = "dlog");

///-------------------------------------------------------------------------------------------------
/// <summary> 关闭模块. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_close();

///-------------------------------------------------------------------------------------------------
/// <summary> 得到当前设置的日志目录. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="result"> [out] If non-null, the result. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_get_log_dir(char* result);

///-------------------------------------------------------------------------------------------------
/// <summary> 设置整个log使能. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_enable(bool enable);

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 设置最高某一级日志输出到控制台，大于等于这一级别的日志都会输出到控制台，小于这一级别的日志不会输出到控制台.
/// 参数有DLOG_INFO,DLOG_WARNING,DLOG_ERROR, DLOG_FATAL.
/// </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="LogSeverity"> 大于等于这一级的日志都会输出到控制台. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_FLAGS_stderrthreshold(int LogSeverity);

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an info. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogI(const char * strFormat, ...);

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a warring. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogW(const char * strFormat, ...);

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an error. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogE(const char * strFormat, ...);

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a fatal. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogFATAL(const char * strFormat, ...);

extern "C" DLOG_EXPORT void __stdcall LogTest(int i);