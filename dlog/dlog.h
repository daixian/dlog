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
/// <summary> ģ���ʼ��. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="logDir">  [in]��־�ļ���·���������ģ��Ŀ¼��. </param>
/// <param name="program"> [in]������. </param>
///
/// <returns> ���֮ǰδ����ʼ������0�����򷵻�1. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_init(char* logDir = "\\log", char* program = "dlog");

///-------------------------------------------------------------------------------------------------
/// <summary> �ر�ģ��. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_close();

///-------------------------------------------------------------------------------------------------
/// <summary> �õ���ǰ���õ���־Ŀ¼. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="result"> [out] If non-null, the result. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_get_log_dir(char* result);

///-------------------------------------------------------------------------------------------------
/// <summary> ��������logʹ��. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="enable"> ����Ϊfalse֮��Log������ֱ�ӷ��ز����κβ���. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_enable(bool enable);

///-------------------------------------------------------------------------------------------------
/// <summary>
/// �������ĳһ����־���������̨�����ڵ�����һ�������־�������������̨��С����һ�������־�������������̨.
/// ������DLOG_INFO,DLOG_WARNING,DLOG_ERROR, DLOG_FATAL.
/// </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="LogSeverity"> ���ڵ�����һ������־�������������̨. </param>
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