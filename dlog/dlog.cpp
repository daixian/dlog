// dlog.cpp : 定义 DLL 应用程序的导出函数。
//

#include "dlog.h"

#include "Common/GLogHelper.h"
#include "Common/Debug.h"

using namespace dxlib;

//全局对象，用于在dll释放的时候自动析构
GLogHelper * inst = NULL;

///-------------------------------------------------------------------------------------------------
/// <summary> 模块初始化. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="program"> [in]程序名. </param>
/// <param name="logDir">  [in]日志文件夹路径名（相对模块目录）. </param>
///
/// <returns> 如果之前未被初始化返回0，否则返回1. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_init(char* logDir, char* program)
{
    if (inst == NULL)
    {
        inst = new GLogHelper(program, logDir);
        return 0;
    }

    return 1;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 关闭模块. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_close()
{
    if (inst != NULL)
    {
        delete inst;
        inst = NULL;
        return 0;
    }
    return 1;
}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到当前设置的日志目录. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="result"> [out] If non-null, the result. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_get_log_dir(char* result)
{
    if (inst != NULL)
    {
        inst->logDirPath.copy(result, FLAGS_log_dir.size(), 0);
    }
}

///-------------------------------------------------------------------------------------------------
/// <summary> 设置整个log使能. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="enable"> 设置为false之后Log函数会直接返回不作任何操作. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_enable(bool enable)
{
    Debug::GetInst()->isLog = enable;
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// 设置大于等于某一级日志输出到控制台. 参数有DLOG_INFO,DLOG_WARNING,DLOG_ERROR,
/// DLOG_FATAL.
/// </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="LogSeverity"> 大于等于这一级的日志都会输出到控制台. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_FLAGS_stderrthreshold(int LogSeverity)
{
    FLAGS_stderrthreshold = LogSeverity; // 当日志级别大于等于此级别时，自动将此日志输出到标准错误(终端窗口)中
}

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
extern "C" DLOG_EXPORT void __stdcall LogI(const char * strFormat, ...)
{
    if (inst == NULL)//如果还没有初始化过，那么就调用默认构造
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogI_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a w. </summary>
///
/// <remarks> Dx, 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogW(const char * strFormat, ...)
{
    if (inst == NULL)//如果还没有初始化过，那么就调用默认构造
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogW_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an e. </summary>
///
/// <remarks> Dx, 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogE(const char * strFormat, ...)
{
    if (inst == NULL)//如果还没有初始化过，那么就调用默认构造
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogE_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a fatal. </summary>
///
/// <remarks> Dx                                                                                                                                                                                                                     , 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogFATAL(const char * strFormat, ...)
{
    if (inst == NULL)//如果还没有初始化过，那么就调用默认构造
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogFATAL_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

extern "C" DLOG_EXPORT void __stdcall LogTest(int i)
{
    if (inst == NULL)//如果还没有初始化过，那么就调用默认构造
    {
        dlog_init();
    }

    LOG(INFO) << "测试日志" << i;
}